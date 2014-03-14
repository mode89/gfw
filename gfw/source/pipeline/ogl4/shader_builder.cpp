#include "common/trace.h"
#include "gfw/pipeline/common/parse_tree.h"
#include "gfw/pipeline/shader_builder.h"
#include "gfw/pipeline/validator.h"

#include <algorithm>
#include <cstring>
#include <string>
#include <sstream>

namespace GFW {

    inline static bool CompareTreesByRef( const ConstParseTreeRef & t1, const ConstParseTreeRef & t2 )
    {
        return t1.GetPointer() < t2.GetPointer();
    }

    inline static bool LessByTreeRef( const Symbol * ls, const Symbol * rs )
    {
        return ls->GetTree().GetPointer() < rs->GetTree().GetPointer();
    }

    class ConstructSourceVisitor
    {
    public:
        ConstructSourceVisitor(
            std::stringstream & source,
            const TextureSamplerPairSet & textureSamplerPairSet,
            ConstSymbolTableIn symbolTable,
            const Symbol * entryPoint )
            : mSource( source )
            , mLine( 1 )
            , mRow( 0 )
            , mTextureSamplerPairSet( textureSamplerPairSet )
        {
            for ( SymbolTable::const_iterator it = symbolTable->begin(); it != symbolTable->end(); ++ it )
            {
                const Symbol & symbol = *it;
                if ( !entryPoint->RefersTo( &symbol ) )
                {
                    mSkipSymbols.push_back( &symbol );
                }
            }
            std::sort( mSkipSymbols.begin(), mSkipSymbols.end(), LessByTreeRef );
        }

        bool operator() ( ConstParseTreeRef tree )
        {
            // If an unused symbol
            {
                const Symbol symbol( tree );
                if ( std::binary_search( mSkipSymbols.begin(), mSkipSymbols.end(), &symbol, LessByTreeRef ) )
                {
                    AdvancePosition( tree );
                    return false;
                }
            }

            switch ( tree->GetTokenType() )
            {
            case TOKEN_VARIABLE_DEFINITION:
                if ( tree->GetChild()->GetTokenType() == TOKEN_TEXTURE2D )
                {
                    PutLeadingWhitespaces( tree );
                    for ( TextureSamplerPairSet::iterator it = mTextureSamplerPairSet.begin(); it != mTextureSamplerPairSet.end(); ++ it )
                    {
                        if ( it->first->GetTree() == tree )
                        {
                            mSource << "uniform sampler2D _sampler_"
                                    << it->first->GetName()
                                    << "_"
                                    << it->second->GetName()
                                    << ";";
                        }
                    }
                    AdvancePosition( tree );
                    return false;
                }
                break;
            case TOKEN_TEXTURE_SAMPLE_EXPRESSION_HEAD:
                {
                    PutLeadingWhitespaces( tree );
                    const char * textureName = tree->GetFirstChildWithType( TOKEN_TEXTURE_OBJECT_ID )->GetChild()->ToString();
                    const char * samplerName = tree->GetFirstChildWithType( TOKEN_SAMPLER_OBJECT_ID )->GetChild()->ToString();
                    mSource << "texture( "
                            << "_sampler_"
                            << textureName
                            << "_"
                            << samplerName;
                    AdvancePosition( tree );
                }
                return false;
            case TOKEN_TECHNIQUE_DEFINITION:
            case TOKEN_SEMANTIC:
            case TOKEN_STATE_OBJECT_DEFINITION:
                AdvancePosition( tree );
                return false;
            default:
                {
                    std::string token = tree->ToString();
                    if ( !token.empty() )
                    {
                        PutLeadingWhitespaces( tree );
                        mSource << token;
                        AdvancePosition( tree );
                    }
                }
                return true;
            }

            return true;
        }

    private:
        void PutLeadingWhitespaces( ConstParseTreeIn tree )
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

        void AdvancePosition( ConstParseTreeIn tree )
        {
            mLine = tree->GetEndLine();
            mRow = tree->GetEndRow();
        }

    private:
        std::stringstream &             mSource;
        uint32_t                        mLine;
        uint32_t                        mRow;

        const TextureSamplerPairSet &   mTextureSamplerPairSet;
        SymbolReferenceVec              mSkipSymbols;
    };

    class CollectTextureSampleExpressionsVisitor
    {
    public:
        CollectTextureSampleExpressionsVisitor(
            ConstSymbolTableIn symbolTable,
            TextureSamplerPairSet & textureSamplerPairSet )
            : mSymbolTable( symbolTable )
            , mTextureSamplerPairSet( textureSamplerPairSet )
        {}

        bool operator() ( ConstParseTreeIn tree )
        {
            if ( tree->GetTokenType() == TOKEN_TEXTURE_SAMPLE_EXPRESSION_HEAD )
            {
                ConstParseTreeRef textureObjectId = tree->GetFirstChildWithType( TOKEN_TEXTURE_OBJECT_ID )->GetChild();
                ConstParseTreeRef samplerObjectId = tree->GetFirstChildWithType( TOKEN_SAMPLER_OBJECT_ID )->GetChild();

                SymbolReferenceVec textureSymbol;
                if ( !mSymbolTable->LookupSymbolByName( textureObjectId->ToString(), textureSymbol ) )
                {
                    TRACE_ERROR_FORMATTED( "Failed to find texture object '%s' in the global scope.", textureObjectId->ToString() );
                }

                SymbolReferenceVec samplerSymbol;
                if ( !mSymbolTable->LookupSymbolByName( samplerObjectId->ToString(), samplerSymbol ) )
                {
                    TRACE_ERROR_FORMATTED( "Failed to find sampler object '%s' in the global scope.", samplerObjectId->ToString() );
                }

                TextureSamplerPair textureSamplerPair( textureSymbol[0], samplerSymbol[0] );
                mTextureSamplerPairSet.insert( textureSamplerPair );

                return false;
            }
            return true;
        }

    private:
        ConstSymbolTableRef     mSymbolTable;
        TextureSamplerPairSet & mTextureSamplerPairSet;
    };

    template <typename Handler>
    void EnumInputsOutputs( const Symbol * entryPoint, const Handler & handler )
    {
        // Process return value

        if ( entryPoint->GetType()->GetTokenType() != TOKEN_VOID )
        {
            handler( false, entryPoint->GetType()->ToString(), "retval", entryPoint->GetSemantic() );
        }

        // Process arguments

        const ParseTreeVec & args = entryPoint->GetArgs();
        for ( ParseTreeVec::const_iterator it = args.begin(); it != args.end(); ++ it )
        {
            ConstParseTreeRef arg = *it;

            ConstParseTreeRef type     = arg->GetFirstChildWithType( TOKEN_TYPE_SPECIFIER );
            ConstParseTreeRef name     = arg->GetFirstChildWithType( TOKEN_ID );
            ConstParseTreeRef semantic = arg->GetFirstChildWithType( TOKEN_SEMANTIC );

            handler( true, type->GetChild()->ToString(), name->ToString(),
                semantic.IsAttached() ? semantic->GetChild()->ToString() : nullptr );
        }
    }

    class ExpandInputOutput
    {
    public:
        ExpandInputOutput( std::ostream & stream, ConstSymbolTableIn symbolTable )
            : mStream( stream )
            , mSymbolTable( symbolTable )
        {}

        void operator() ( bool isInput, const char * type, const char * name, const char * semantic ) const
        {
            mNames.push_back( name );

            SymbolReferenceVec typeSymbol;
            if ( mSymbolTable->LookupSymbolByName( type, typeSymbol ) && typeSymbol[0]->IsStruct() )
            {
                const ParseTreeVec & members = typeSymbol[0]->GetMembers();
                for ( ParseTreeVec::const_iterator it = members.begin(); it != members.end(); ++ it )
                {
                    ConstParseTreeRef m = *it;

                    ConstParseTreeRef type = m->GetFirstChildWithType( TOKEN_TYPE_SPECIFIER );
                    ConstParseTreeRef name = m->GetFirstChildWithType( TOKEN_ID );
                    ConstParseTreeRef semantic = m->GetFirstChildWithType( TOKEN_SEMANTIC );

                    (*this)( isInput, type->GetChild()->ToString(), name->ToString(),
                        semantic.IsAttached() ? semantic->GetChild()->ToString() : nullptr );
                }
            }
            else
            {
                Handler( mStream, isInput, type, mNames, semantic );
            }

            mNames.pop_back();
        }

    protected:
        typedef std::vector<const char *> Names;

        virtual void
        Handler( std::ostream &, bool isInput, const char * type, const Names &, const char * semantic ) const = 0;

    private:
        std::ostream &      mStream;
        ConstSymbolTableRef mSymbolTable;
        mutable Names       mNames;
    };

    class ExpandOutputAsGlobalDeclaration : public ExpandInputOutput
    {
    public:
        ExpandOutputAsGlobalDeclaration( std::ostream & stream, ConstSymbolTableIn symboTable, ShaderStage stage )
            : ExpandInputOutput( stream, symboTable )
            , mStage( stage )
        {}

        void Handler( std::ostream & stream, bool isInput, const char * typeName, const Names & names, const char * semantic ) const
        {
            if ( !isInput )
            {
                if ( mStage == ShaderStage::VERTEX && std::strcmp( semantic, "SV_POSITION" ) == 0 )
                {
                    // Equivalent gl_Position
                    return;
                }
                else
                {
                    // OpenGL doesn't allow output blocks in fragment shader
                    stream << ( ( mStage == ShaderStage::PIXEL ) ? "out " : "    " );
                    stream << typeName << ' ' << semantic;
                }
                stream << ';' << std::endl;
            }
        }

    private:
        ShaderStage mStage;
    };

    class ExpandInputAsGlobalDeclaration : public ExpandInputOutput
    {
    public:
        ExpandInputAsGlobalDeclaration( std::ostream & stream, ConstSymbolTableIn symboTable, ShaderStage stage )
            : ExpandInputOutput( stream, symboTable )
            , mStage( stage )
        {}

        void Handler( std::ostream & stream, bool isInput, const char * typeName, const Names & names, const char * semantic ) const
        {
            if ( isInput )
            {
                if ( mStage == ShaderStage::PIXEL && strcmp( semantic, "SV_POSITION") == 0 )
                {
                    // Equivalent gl_FragCoord;
                    return;
                }
                else
                {
                    // OpenGL doesn't allow input blocks in vertex shader
                    stream << ( ( mStage == ShaderStage::VERTEX ) ? "in " : "    " );
                    stream << typeName << ' ' << semantic << ';' << std::endl;
                }
            }
        }

    private:
        ShaderStage mStage;
    };

    class ExpandInputOutputAsBuiltinDeclaration : public ExpandInputOutput
    {
    public:
        ExpandInputOutputAsBuiltinDeclaration( std::ostream & stream, ConstSymbolTableIn symbolTable, ShaderStage stage )
            : ExpandInputOutput( stream, symbolTable )
            , mStage( stage )
        {}

        void Handler( std::ostream & stream, bool isInput, const char * typeName, const Names & names, const char * semantic ) const
        {
            if ( !isInput )
            {
                if ( mStage == ShaderStage::VERTEX && std::strcmp( semantic, "SV_POSITION" ) == 0 )
                {
                    stream << "out gl_PerVertex { " << typeName << " gl_Position; };" << std::endl;
                }
            }
        }

    private:
        ShaderStage mStage;
    };

    class ExpandInputOutputAsLocalDeclaration
    {
    public:
        ExpandInputOutputAsLocalDeclaration( std::ostream & stream )
            : mStream( stream )
        {}

        void operator() ( bool isInput, const char * type, const char * name, const char * semantic ) const
        {
            mStream << "    " << type << ' ' << name << ';' << std::endl;
        }

    private:
        std::ostream & mStream;
    };

    class AssignInput : public ExpandInputOutput
    {
    public:
        AssignInput( std::ostream & stream, ConstSymbolTableIn symbolTable, ShaderStage stage )
            : ExpandInputOutput( stream, symbolTable )
            , mStage( stage )
        {}

        void Handler( std::ostream & stream, bool isInput, const char * typeName, const Names & names, const char * semantic ) const
        {
            if ( isInput )
            {
                stream << "    ";

                stream << names[0];
                for ( Names::size_type i = 1; i < names.size(); ++ i )
                {
                    stream << "." << names[i];
                }

                stream << " = ";

                if ( mStage == ShaderStage::VERTEX )
                {
                    // OpenGL doesn't allow input blocks in vertex shader
                    stream << semantic;
                }
                else if ( mStage == ShaderStage::PIXEL && std::strcmp( semantic, "SV_POSITION" ) == 0 )
                {
                    stream << "gl_FragCoord";
                }
                else
                {
                    stream << "_inputs." << semantic;
                }

                stream << ";" << std::endl;
            }
        }

    private:
        ShaderStage mStage;
    };

    class ExpandInputOutputAsEntryPointArgument
    {
    public:
        ExpandInputOutputAsEntryPointArgument( std::ostream & stream, const Symbol * entryPoint )
            : mStream( stream )
            , mIsVoid( entryPoint->GetType()->GetTokenType() == TOKEN_VOID )
            , mEntryName( entryPoint->GetName() )
            , mInputsOutputsNumber( entryPoint->GetArgs().size() + ( mIsVoid ? 0 : 1 ) )
            , mInputsOutputsCounter( 0 )
        {}

        void operator() ( bool isInput, const char * type, const char * name, const char * semantic ) const
        {
            if ( ( mInputsOutputsCounter ++ ) == 0 )
            {
                if ( mIsVoid )
                {
                    mStream << "    ";
                    mStream << mEntryName << "( " << name;
                    mStream << ( ( mInputsOutputsCounter != mInputsOutputsNumber ) ? ", " : " );\n" );
                }
                else
                {
                    mStream << "    ";
                    mStream << name << " = " << mEntryName << "(";
                    mStream << ( ( mInputsOutputsCounter != mInputsOutputsNumber ) ? " " : ");\n" );
                }
            }
            else
            {
                mStream << name;
                mStream << ( ( mInputsOutputsCounter != mInputsOutputsNumber ) ? ", " : " );\n" );
            }
        }

    private:
        std::ostream &      mStream;
        bool                mIsVoid;
        const char *        mEntryName;
        uint32_t            mInputsOutputsNumber;
        mutable uint32_t    mInputsOutputsCounter;
    };

    class AssignOutput : public ExpandInputOutput
    {
    public:
        AssignOutput( std::ostream & stream, ConstSymbolTableIn symbolTable, ShaderStage stage )
            : ExpandInputOutput( stream, symbolTable )
            , mStage( stage )
        {}

        void Handler( std::ostream & stream, bool isInput, const char * typeName, const Names & names, const char * semantic ) const
        {
            if ( !isInput )
            {
                stream << "    ";

                if ( mStage == ShaderStage::VERTEX && std::strcmp( semantic, "SV_POSITION" ) == 0 )
                {
                    stream << "gl_Position";
                }
                else if ( mStage == ShaderStage::PIXEL )
                {
                    // OpenGL doesn't allow output blocks in fragment shader
                    stream << semantic;
                }
                else
                {
                    stream << "_outputs." << semantic;
                }

                stream << " = ";
                stream << names[0];
                for ( Names::size_type i = 1; i < names.size(); ++ i )
                {
                    stream << "." << names[i];
                }

                stream << ";" << std::endl;
            }
        }

    private:
        ShaderStage mStage;
    };

    ShaderBuilder::ShaderBuilder( ConstParseTreeIn tree, ConstSymbolTableIn symbolTable )
        : mParseTree( tree )
        , mSymbolTable( symbolTable )
    {
        AcquireValidator();

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
                for ( SymbolReferenceVec::const_iterator refIt = functionSymbol->GetReferences().begin();
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
        // Query entry point's symbol

        SymbolReferenceVec entryPointReference;
        if ( !mSymbolTable->LookupSymbolByName(shaderName, entryPointReference) )
        {
            TRACE_ERROR_FORMATTED( "Failed to find shader entry point '%s'.", shaderName );
        }
        const Symbol * entryPoint = entryPointReference[0];

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

        source  << "#version 430 core" << std::endl
                << std::endl
                << "#define int2        ivec2" << std::endl
                << "#define int3        ivec3" << std::endl
                << "#define int4        ivec4" << std::endl
                << "#define uint2       uvec2" << std::endl
                << "#define uint3       uvec3" << std::endl
                << "#define uint4       uvec4" << std::endl
                << "#define half2       hvec2" << std::endl
                << "#define half3       hvec3" << std::endl
                << "#define half4       hvec4" << std::endl
                << "#define float2      vec2" << std::endl
                << "#define float3      vec3" << std::endl
                << "#define float4      vec4" << std::endl
                << "#define float2x2    mat2" << std::endl
                << "#define float3x3    mat3" << std::endl
                << "#define float4x4    mat4" << std::endl
                << std::endl
                << "#define mul(a, b) ((a) * (b))" << std::endl
                << std::endl;

        const TextureSamplerPairSet & textureSamplerPairSet = mFunctionTextureSamplerMap[entryPoint];
        ConstructSourceVisitor constructSource( source, textureSamplerPairSet, mSymbolTable, entryPoint );
        mParseTree->TraverseDFS( constructSource );
        source << std::endl << std::endl;

        // Declare built-in variables

        EnumInputsOutputs( entryPoint, ExpandInputOutputAsBuiltinDeclaration( source, mSymbolTable, stage ) );
        source << std::endl;

        // Declare inputs

        std::stringstream inputDecls;
        EnumInputsOutputs( entryPoint, ExpandInputAsGlobalDeclaration( inputDecls, mSymbolTable, stage ) );

        if ( stage == ShaderStage::VERTEX )
        {
            // OpenGL doesn't allow input blocks in vertex shader
            source << inputDecls.str();
            source << std::endl;
        }
        else
        {
            if ( !inputDecls.str().empty() )
            {
                source << "in _INOUTS {" << std::endl;
                source << inputDecls.str();
                source << "} _inputs;" << std::endl;
                source << std::endl;
            }
        }

        // Declare outputs

        std::stringstream outputDecls;
        EnumInputsOutputs( entryPoint, ExpandOutputAsGlobalDeclaration( outputDecls, mSymbolTable, stage ) );

        if ( stage == ShaderStage::PIXEL )
        {
            // OpenGL doesn't allow output blocks in fragment shader
            source << outputDecls.str();
            source << std::endl;
        }
        else
        {
            if ( !outputDecls.str().empty() )
            {
                source << "out _INOUTS {" << std::endl;
                source << outputDecls.str();
                source << "} _outputs;" << std::endl;
                source << std::endl;
            }
        }

        // main()

        source << "void main()\n{\n";
        {
            // Declare local copies of inputs and outputs

            EnumInputsOutputs( entryPoint, ExpandInputOutputAsLocalDeclaration( source ) );

            // Assign the inputs

            EnumInputsOutputs( entryPoint, AssignInput( source, mSymbolTable, stage ) );

            // Call the entry point

            EnumInputsOutputs( entryPoint, ExpandInputOutputAsEntryPointArgument( source, entryPoint ) );

            // Assign outputs

            EnumInputsOutputs( entryPoint, AssignOutput( source, mSymbolTable, stage ) );
        }
        source << "}\n";

        Validate( stage, source.str().c_str() );

        // Construct binary

        ShaderBinaryRef shaderBinary = new ShaderBinary;

        shaderBinary->mDesc.inputsCount = entryPoint->GetArgs().size();
        shaderBinary->mStage = stage;

        uint32_t sourceSize = source.str().size() + 1; // + 1 for null-terminator
        shaderBinary->mSize = sourceSize;
        shaderBinary->mData = new uint8_t [ sourceSize ];
        std::memcpy( shaderBinary->mData, source.str().c_str(), sourceSize );

        return shaderBinary;
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
