#include "common/trace.h"
#include "gfw/pipeline/common/parse_tree.h"
#include "gfw/pipeline/shader_builder.h"
#include "gfw/pipeline/validator.h"

#include <algorithm>
#include <cstring>
#include <string>
#include <sstream>

namespace GFW {

    typedef std::vector< ConstParseTreeRef > ParseTreeVec;

    inline static bool CompareTreesByRef( const ConstParseTreeRef & t1, const ConstParseTreeRef & t2 )
    {
        return t1.GetPointer() < t2.GetPointer();
    }

    class ConstructSourceVisitor
    {
    public:
        ConstructSourceVisitor( std::stringstream & source, const ParseTreeVec & skipNodes )
            : mSource( source )
            , mSkipNodes( skipNodes )
            , mLine( 1 )
            , mRow( 0 )
        {}

        bool operator() ( ConstParseTreeRef tree )
        {
            TokenType tokenType = tree->GetTokenType();
            std::string token = tree->ToString();

            // Prepend the token with necessary whitespaces
            {
                uint32_t line = tree->GetLine();
                uint32_t row  = tree->GetRow();

                for ( ; mLine < line; ++ mLine )
                {
                    mSource << std::endl;
                    mRow = 0;
                }

                for ( ; mRow < row; ++ mRow )
                {
                    mSource << ' ';
                }
            }

            // Skip node
            if ( std::binary_search( mSkipNodes.begin(), mSkipNodes.end(), tree, CompareTreesByRef ) )
            {
                return false;
            }

            TranslateIntrinsicType( token, tokenType );

            // Output token if has symbolic representation
            if ( !token.empty() )
            {
                mSource << token;

                // Move output position to the end of the token
                {
                    mLine = tree->GetEndLine();
                    mRow = tree->GetEndRow();
                }
            }

            return true;
        }

    private:

        void TranslateIntrinsicType( std::string & token, TokenType tokenType )
        {
#define TYPES \
    T( INT2, ivec2 ) \
    T( INT3, ivec3 ) \
    T( INT4, ivec4 ) \
    T( UINT2, uvec2 ) \
    T( UINT3, uvec3 ) \
    T( UINT4, uvec4 ) \
    T( HALF2, hvec2 ) \
    T( HALF3, hvec3 ) \
    T( HALF4, hvec4 ) \
    T( FLOAT2, vec2 ) \
    T( FLOAT3, vec3 ) \
    T( FLOAT4, vec4 ) \
    T( FLOAT22, mat2 ) \
    T( FLOAT33, mat3 ) \
    T( FLOAT44, mat4 ) \

            switch ( tokenType )
            {
                #define T( type, gltype ) case TOKEN_ ## type : token = #gltype; return;
                    TYPES
                #undef T
            }

#undef TYPES
        }

    private:
        std::stringstream &     mSource;
        const ParseTreeVec &    mSkipNodes;
        uint32_t                mLine;
        uint32_t                mRow;
    };

    class CollectTextureSampleExpressionsVisitor
    {
    public:
        CollectTextureSampleExpressionsVisitor(
            ConstSymbolTableIn symbolTable,
            ShaderBuilder::TextureSamplerPairSet & textureSamplerPairSet )
            : mSymbolTable( symbolTable )
            , mTextureSamplerPairSet( textureSamplerPairSet )
        {}

        bool operator() ( ConstParseTreeIn tree )
        {
            if ( tree->GetTokenType() == TOKEN_TEXTURE_SAMPLE_EXPRESSION )
            {
                ConstParseTreeRef textureObjectId = tree->GetFirstChildWithType( TOKEN_TEXTURE_OBJECT_ID )->GetChild();
                ConstParseTreeRef samplerObjectId = tree->GetFirstChildWithType( TOKEN_SAMPLER_OBJECT_ID )->GetChild();

                Symbol::References textureSymbol;
                if ( !mSymbolTable->LookupSymbolByName( textureObjectId->ToString(), textureSymbol ) )
                {
                    TRACE_ERROR_FORMATTED( "Failed to find texture object '%s' in the global scope.", textureObjectId->ToString() );
                }

                Symbol::References samplerSymbol;
                if ( !mSymbolTable->LookupSymbolByName( samplerObjectId->ToString(), samplerSymbol ) )
                {
                    TRACE_ERROR_FORMATTED( "Failed to find sampler object '%s' in the global scope.", samplerObjectId->ToString() );
                }

                ShaderBuilder::TextureSamplerPair textureSamplerPair( textureSymbol[0], samplerSymbol[0] );
                mTextureSamplerPairSet.insert( textureSamplerPair );

                return false;
            }
            return true;
        }

    private:
        ConstSymbolTableRef                     mSymbolTable;
        ShaderBuilder::TextureSamplerPairSet &  mTextureSamplerPairSet;
    };

    ShaderBuilder::ShaderBuilder( ConstParseTreeIn tree, ConstSymbolTableIn symbolTable )
        : mParseTree( tree )
        , mSymbolTable( symbolTable )
    {
        AcquireValidator();

        mParseTree->TraverseDFS( *this, &ShaderBuilder::CollectFunctions );

        mParseTree->TraverseDFS( *this, &ShaderBuilder::CollectFXNodes );
        std::sort( mFXNodes.begin(), mFXNodes.end(), CompareTreesByRef );

        mParseTree->TraverseDFS( *this, &ShaderBuilder::CollectVariables );

        // Collect immediate references to sampling expressions

        for ( SymbolTable::const_iterator it = mSymbolTable->begin(); it != mSymbolTable->end(); ++ it )
        {
            if ( it->IsFunction() )
            {
                const Symbol * functionSymbol = &(*it);
                TextureSamplerPairSet & textureSamplerPairSet = mFunctionTextureSamplerMap[functionSymbol];
                CollectTextureSampleExpressionsVisitor collectTextureSampleExpressions( mSymbolTable, textureSamplerPairSet );
                it->GetTree()->TraverseDFS( collectTextureSampleExpressions );
            }
        }

        // Collect indirect references to sampling expressions

        for ( SymbolTable::const_iterator it = mSymbolTable->begin(); it != mSymbolTable->end(); ++ it )
        {
            if ( it->IsFunction() )
            {
                const Symbol * functionSymbol = &(*it);
                TextureSamplerPairSet & functionTextureSamplerPairSet = mFunctionTextureSamplerMap[functionSymbol];
                for ( Symbol::References::const_iterator refIt = functionSymbol->GetReferences().begin();
                      refIt != functionSymbol->GetReferences().end(); ++ refIt )
                {
                    const Symbol * referencedFunctionSymbol = *refIt;
                    if ( referencedFunctionSymbol->IsFunction() && referencedFunctionSymbol != functionSymbol )
                    {
                        TextureSamplerPairSet & referencedFunctionTextureSamplerPairSet =
                            mFunctionTextureSamplerMap[referencedFunctionSymbol];
                        functionTextureSamplerPairSet.insert( referencedFunctionTextureSamplerPairSet.begin(),
                            referencedFunctionTextureSamplerPairSet.end() );
                    }
                }
            }
        }
    }

    ShaderBuilder::~ShaderBuilder()
    {
        ReleaseValidator();
    }

    ShaderBinaryRef ShaderBuilder::Compile( const char * shaderName, const char * profile )
    {
        // Translate the profile

        ShaderStage stage;
        if ( strcmp( profile, "vs_4_0" ) == 0 )
        {
            stage = ShaderStage::VERTEX;
        }
        else if ( strcmp( profile, "ps_4_0" ) == 0 )
        {
            stage = ShaderStage::PIXEL;
        }
        else
        {
            stage = ShaderStage::UNKNOWN;
        }

        // Construct the shader's code

        std::stringstream source;

        source  << "#version 430 core"
                << std::endl
                << std::endl
                << "#define float4  vec4"
                << std::endl
                << std::endl;

        ParseTreeVec skipNodes = mFXNodes;
        ConstructSourceVisitor constructSource( source, skipNodes );
        mParseTree->TraverseDFS( constructSource );

        source << std::endl << std::endl;

        FunctionMap::iterator it = mFunctions.find( shaderName );
        TRACE_ASSERT( it != mFunctions.end() );
        Function & entryPoint = it->second;

        // Input parameters

        for ( uint32_t i = 0; i < entryPoint.args.size(); ++ i )
        {
            ConstParseTreeRef arg = entryPoint.args[i];
            source << "in ";
            source << arg->GetChild( 0 )->ToString();
            source << " _in_";
            source << i;
            source << "; // ";
            source << arg->GetChild( 1 )->ToString();

            ConstParseTreeRef semantic = arg->GetFirstChildWithType( TOKEN_SEMANTIC );
            if ( semantic.IsAttached() )
            {
                source << " : ";
                source << semantic->GetChild()->ToString();
            }

            source << std::endl;
        }
        source << std::endl;

        // Output parameters

        if ( stage == ShaderStage::VERTEX )
        {
            source  << "out gl_PerVertex {" << std::endl
                    << "    vec4 gl_Position;" << std::endl
                    << "};" << std::endl << std::endl;
        }

        // main()

        source << "void main()\n{\n    ";
        {
            // Call the shader

            if ( entryPoint.ret->GetTokenType() != TOKEN_VOID )
            {
                source << entryPoint.ret->ToString();
                source << " outp = ";
            }

            source << shaderName;
            source << "(";
            if ( entryPoint.args.size() != 0 )
            {
                source << ' ';
                for ( uint32_t i = 0; i < entryPoint.args.size(); ++ i )
                {
                    if ( i != 0 ) source << ", ";
                    source << "_in_";
                    source << i;
                }
                source << ' ';
            }
            source << ");\n";

            // Assign outputs

            if ( entryPoint.ret->GetTokenType() != TOKEN_VOID )
            {
                if ( entryPoint.sem.IsAttached() )
                {
                    source << "    ";
                    ConstParseTreeRef semantic = entryPoint.sem->GetChild();
                    if ( std::strcmp( "SV_POSITION", semantic->ToString() ) == 0 )
                    {
                        source << "gl_Position = outp;\n";
                    }
                    else if ( std::strcmp( "SV_TARGET", semantic->ToString() ) == 0 )
                    {
                        source << "gl_FragColor = outp;\n";
                    }
                }
            }
        }
        source << "}\n";

        Validate( stage, source.str().c_str() );

        // Construct binary

        ShaderBinaryRef shaderBinary = new ShaderBinary;

        shaderBinary->mDesc.inputsCount = entryPoint.args.size();
        shaderBinary->mStage = stage;

        uint32_t sourceSize = source.str().size() + 1; // + 1 for null-terminator
        shaderBinary->mSize = sourceSize;
        shaderBinary->mData = new uint8_t [ sourceSize ];
        std::memcpy( shaderBinary->mData, source.str().c_str(), sourceSize );

        return shaderBinary;
    }

    bool ShaderBuilder::CollectFunctions( ConstParseTreeIn tree )
    {
        if ( tree->GetTokenType() == TOKEN_EXTERNAL_DECLARATION )
        {
            ConstParseTreeRef child = tree->GetChild();
            if ( child->GetTokenType() == TOKEN_FUNCTION_DEFINITION )
            {
                ConstParseTreeRef name = child->GetFirstChildWithType( TOKEN_SYMBOL_NAME )->GetChild();

                Function & func = mFunctions[ name->ToString() ];

                func.tree = child;
                func.ret  = child->GetChild();
                func.sem  = child->GetFirstChildWithType( TOKEN_SEMANTIC );

                // Collect arguments

                ConstParseTreeRef args = child->GetFirstChildWithType( TOKEN_ARGUMENTS_LIST );
                if ( args.IsAttached() )
                {
                    for ( uint32_t i = 0; i < args->GetChildCount(); ++ i )
                    {
                        if ( args->GetChild(i)->GetTokenType() == TOKEN_ARGUMENT )
                        {
                            func.args.push_back( args->GetChild(i) );
                        }
                    }
                }
            }
            return false;
        }
        return true;
    }

    bool ShaderBuilder::CollectFXNodes( ConstParseTreeIn tree )
    {
        if ( tree->GetTokenType() == TOKEN_TECHNIQUE_DEFINITION ||
             tree->GetTokenType() == TOKEN_SEMANTIC )
        {
            mFXNodes.push_back( tree );
            return false;
        }
        return true;
    }

    bool ShaderBuilder::CollectVariables( ConstParseTreeIn tree )
    {
        if ( tree->GetTokenType() == TOKEN_EXTERNAL_DECLARATION )
        {
            ConstParseTreeRef child = tree->GetChild();
            if ( child->GetTokenType() == TOKEN_VARIABLE_DEFINITION ||
                 child->GetTokenType() == TOKEN_STATE_OBJECT_DEFINITION )
            {
                ConstParseTreeRef name = child->GetFirstChildWithType( TOKEN_SYMBOL_NAME )->GetChild();
                mVariables[name->ToString()] = child;
            }
            return false;
        }
        return true;
    }

} // namespace GFW
