#include "cmn/platform.h"
#include "cmn/trace.h"

CMN_WARNING_PUSH
CMN_WARNING_DISABLE_MSVC( 4242 4265 4310 4619 4625 4626 )
CMN_WARNING_DISABLE_GCC( unused-local-typedefs )
#include "boost/archive/binary_oarchive.hpp"
#include "boost/serialization/utility.hpp"
#include "boost/serialization/vector.hpp"
CMN_WARNING_POP

#include "gfw/shader_stage.h"
#include "gfw/pipeline/common/parse_tree.h"
#include "gfw/pipeline/common/symbol.h"
#include "gfw/pipeline/ogl4/shader_builder.h"
#include "gfw/pipeline/ogl4/shader_builder_exception.h"
#include "gfw/pipeline/ogl4/validator.h"
#include "gfw/runtime/ogl4/limits.h"
#include "gfw/shared/shader.h"
#include "gfw/shared/ogl4/shader.h"

#include <algorithm>
#include <cstring>
#include <string>
#include <sstream>

namespace GFW {

    static std::string CreateTextureSamplerName(
        const std::string & textureName,
        const std::string & samplerName )
    {
        return std::string( "_sampler" )
            + std::string( "_" ) + textureName
            + std::string( "_" ) + samplerName;
    }

    class ConstructSourceVisitor
    {
    public:
        ConstructSourceVisitor(
            std::stringstream & source,
            const TextureSamplerPairSet & textureSamplerPairSet,
            const NameSymbolMap & nameSymbolMap,
            const Symbol * entryPoint )
            : mSource( source )
            , mLine( 1 )
            , mRow( 0 )
            , mTextureSamplerPairSet( textureSamplerPairSet )
        {
            for ( auto & it : nameSymbolMap )
            {
                const Symbol & symbol = *it.second;
                if ( entryPoint->references.count( symbol.name ) == 0 )
                {
                    mSkipSymbols.insert( std::make_pair( symbol.tree, &symbol ) );
                }
            }
        }

        bool operator() ( const ParseTree & tree )
        {
            // If an unused symbol
            {
                if ( mSkipSymbols.find( &tree ) != mSkipSymbols.end() )
                {
                    AdvancePosition( tree );
                    return false;
                }
            }

            switch ( tree.GetTokenType() )
            {
            case TOKEN_VARIABLE_DEFINITION:
                if ( tree.GetChild().GetTokenType() == TOKEN_TEXTURE2D )
                {
                    PutLeadingWhitespaces( tree );
                    for ( auto & textureSampler : mTextureSamplerPairSet )
                    {
                        if ( textureSampler.first->tree == &tree )
                        {
                            mSource << "uniform sampler2D "
                                << CreateTextureSamplerName(
                                    textureSampler.first->name,
                                    textureSampler.second->name )
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
                    const std::string & textureName = tree.GetFirstChildWithType( TOKEN_TEXTURE_OBJECT_ID )->GetChild().GetText();
                    const std::string & samplerName = tree.GetFirstChildWithType( TOKEN_SAMPLER_OBJECT_ID )->GetChild().GetText();
                    mSource << "texture( "
                        << CreateTextureSamplerName( textureName, samplerName );
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
                    std::string token = tree.GetText();
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
        void PutLeadingWhitespaces( const ParseTree & tree )
        {
            uint32_t line = tree.GetLine();
            uint32_t row  = tree.GetRow();

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

        void AdvancePosition( const ParseTree & tree )
        {
            mLine = tree.GetEndLine();
            mRow = tree.GetEndRow();
        }

        ConstructSourceVisitor & operator= ( const ConstructSourceVisitor & );

    private:
        std::stringstream &             mSource;
        uint32_t                        mLine;
        uint32_t                        mRow;

        const TextureSamplerPairSet &   mTextureSamplerPairSet;
        ParseTreeSymbolMap              mSkipSymbols;
    };

    class CollectTextureSampleExpressionsVisitor
    {
    public:
        CollectTextureSampleExpressionsVisitor(
            const NameSymbolMap & nameSymbolMap,
            TextureSamplerPairSet & textureSamplerPairSet )
            : mNameSymbolMap( nameSymbolMap )
            , mTextureSamplerPairSet( textureSamplerPairSet )
        {}

        bool operator() ( const ParseTree & tree )
        {
            if ( tree.GetTokenType() == TOKEN_TEXTURE_SAMPLE_EXPRESSION_HEAD )
            {
                const ParseTree & textureObjectId = tree.GetFirstChildWithType( TOKEN_TEXTURE_OBJECT_ID )->GetChild();
                const ParseTree & samplerObjectId = tree.GetFirstChildWithType( TOKEN_SAMPLER_OBJECT_ID )->GetChild();

                auto textureSymbol = mNameSymbolMap.find( textureObjectId.GetText() );
                CMN_THROW_IF( textureSymbol == mNameSymbolMap.end(),
                    ShaderBuilderException::UndefinedTextureObject( textureObjectId.GetText().c_str() ) );

                auto samplerSymbol = mNameSymbolMap.find( samplerObjectId.GetText() );
                CMN_THROW_IF( samplerSymbol == mNameSymbolMap.end(),
                    ShaderBuilderException::UndefinedTextureObject( textureObjectId.GetText().c_str() ) );

                TextureSamplerPair textureSamplerPair( textureSymbol->second, samplerSymbol->second );
                mTextureSamplerPairSet.insert( textureSamplerPair );

                return false;
            }
            return true;
        }

        CollectTextureSampleExpressionsVisitor & operator= ( const CollectTextureSampleExpressionsVisitor & );

    private:
        const NameSymbolMap &   mNameSymbolMap;
        TextureSamplerPairSet & mTextureSamplerPairSet;
    };

    template <typename Handler>
    void EnumInputsOutputs( const Symbol * entryPoint, const Handler & handler )
    {
        // Process return value

        if ( entryPoint->type->GetTokenType() != TOKEN_VOID )
        {
            handler( false, entryPoint->type->GetText(), "retval",
                entryPoint->semantic ? entryPoint->semantic : "" );
        }

        // Process arguments

        auto & args = entryPoint->args;
        for ( auto it = args.begin(); it != args.end(); ++ it )
        {
            const ParseTree * arg = *it;

            const ParseTree * type     = arg->GetFirstChildWithType( TOKEN_TYPE_SPECIFIER );
            const ParseTree * name     = arg->GetFirstChildWithType( TOKEN_ID );
            const ParseTree * semantic = arg->GetFirstChildWithType( TOKEN_SEMANTIC );

            handler( true, type->GetChild().GetText(), name->GetText(),
                semantic ? semantic->GetChild().GetText() : "" );
        }
    }

    class ExpandInputOutput
    {
    public:
        ExpandInputOutput( std::ostream & stream, const NameSymbolMap & nameSymbolMap )
            : mStream( stream )
            , mNameSymbolMap( nameSymbolMap )
        {}

        virtual ~ExpandInputOutput() {}

        void operator() ( bool isInput, const std::string & type, const std::string & name, const std::string & semantic ) const
        {
            mNames.push_back( name );

            auto typeSymbolIt = mNameSymbolMap.find( type );
            if ( typeSymbolIt != mNameSymbolMap.end() && typeSymbolIt->second->isStruct )
            {
                auto & typeSymbol = typeSymbolIt->second;
                auto & members = typeSymbol->members;
                for ( auto it = members.begin(); it != members.end(); ++ it )
                {
                    const ParseTree * m = *it;

                    const ParseTree * type = m->GetFirstChildWithType( TOKEN_TYPE_SPECIFIER );
                    const ParseTree * name = m->GetFirstChildWithType( TOKEN_ID );
                    const ParseTree * semantic = m->GetFirstChildWithType( TOKEN_SEMANTIC );

                    (*this)( isInput, type->GetChild().GetText(), name->GetText(),
                        semantic ? semantic->GetChild().GetText() : "" );
                }
            }
            else
            {
                CMN_THROW_IF( semantic == "", ShaderBuilderException::ParameterWithoutSemantic() );

                DoExpand( mStream, isInput, type, mNames, semantic );
            }

            mNames.pop_back();
        }

        ExpandInputOutput & operator= ( const ExpandInputOutput & );

    protected:
        typedef std::vector< std::string > Names;

        virtual void
        DoExpand( std::ostream &, bool isInput, const std::string & type, const Names &, const std::string & semantic ) const = 0;

    private:
        std::ostream &          mStream;
        const NameSymbolMap &   mNameSymbolMap;
        mutable Names           mNames;
    };

    class ExpandOutputAsGlobalDeclaration : public ExpandInputOutput
    {
    public:
        ExpandOutputAsGlobalDeclaration( std::ostream & stream, const NameSymbolMap & nameSymbolMap, ShaderStage stage )
            : ExpandInputOutput( stream, nameSymbolMap )
            , mStage( stage )
        {}

        void DoExpand( std::ostream & stream, bool isInput, const std::string & typeName, const Names & names, const std::string & semantic ) const
        {
            if ( !isInput )
            {
                if ( mStage == SHADER_STAGE_VERTEX && semantic == "SV_POSITION" )
                {
                    // Equivalent gl_Position
                    return;
                }
                else
                {
                    // OpenGL doesn't allow output blocks in fragment shader
                    stream << ( ( mStage == SHADER_STAGE_PIXEL ) ? "out " : "    " );
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
        ExpandInputAsGlobalDeclaration( std::ostream & stream, const NameSymbolMap & nameSymbolMap, ShaderStage stage )
            : ExpandInputOutput( stream, nameSymbolMap )
            , mStage( stage )
        {}

        void DoExpand( std::ostream & stream, bool isInput, const std::string & typeName, const Names & names, const std::string & semantic ) const
        {
            if ( isInput )
            {
                if ( mStage == SHADER_STAGE_PIXEL && semantic == "SV_POSITION" )
                {
                    // Equivalent gl_FragCoord;
                    return;
                }
                else
                {
                    // OpenGL doesn't allow input blocks in vertex shader
                    stream << ( ( mStage == SHADER_STAGE_VERTEX ) ? "in " : "    " );
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
        ExpandInputOutputAsBuiltinDeclaration( std::ostream & stream, const NameSymbolMap & nameSymbolMap, ShaderStage stage )
            : ExpandInputOutput( stream, nameSymbolMap )
            , mStage( stage )
        {}

        void DoExpand( std::ostream & stream, bool isInput, const std::string & typeName, const Names & names, const std::string & semantic ) const
        {
            if ( !isInput )
            {
                if ( mStage == SHADER_STAGE_VERTEX && semantic == "SV_POSITION" )
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

        void operator() ( bool isInput, const std::string & type, const std::string & name, const std::string & semantic ) const
        {
            mStream << "    " << type << ' ' << name << ';' << std::endl;
        }

        ExpandInputOutputAsLocalDeclaration & operator= ( const ExpandInputOutputAsLocalDeclaration & );

    private:
        std::ostream & mStream;
    };

    class AssignInput : public ExpandInputOutput
    {
    public:
        AssignInput( std::ostream & stream, const NameSymbolMap & nameSymbolMap, ShaderStage stage )
            : ExpandInputOutput( stream, nameSymbolMap )
            , mStage( stage )
        {}

        void DoExpand( std::ostream & stream, bool isInput, const std::string & typeName, const Names & names, const std::string & semantic ) const
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

                if ( mStage == SHADER_STAGE_VERTEX )
                {
                    // OpenGL doesn't allow input blocks in vertex shader
                    stream << semantic;
                }
                else if ( mStage == SHADER_STAGE_PIXEL && semantic == "SV_POSITION" )
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
            , mIsVoid( entryPoint->type->GetTokenType() == TOKEN_VOID )
            , mEntryName( entryPoint->name )
            , mInputsOutputsNumber( entryPoint->args.size() + ( mIsVoid ? 0 : 1 ) )
            , mInputsOutputsCounter( 0 )
        {}

        void operator() ( bool isInput, const std::string & type, const std::string & name, const std::string & semantic ) const
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

        ExpandInputOutputAsEntryPointArgument & operator= ( const ExpandInputOutputAsEntryPointArgument & );

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
        AssignOutput( std::ostream & stream, const NameSymbolMap & nameSymbolMap, ShaderStage stage )
            : ExpandInputOutput( stream, nameSymbolMap )
            , mStage( stage )
        {}

        void DoExpand( std::ostream & stream, bool isInput, const std::string & typeName, const Names & names, const std::string & semantic ) const
        {
            if ( !isInput )
            {
                stream << "    ";

                if ( mStage == SHADER_STAGE_VERTEX && semantic == "SV_POSITION" )
                {
                    stream << "gl_Position";
                }
                else if ( mStage == SHADER_STAGE_PIXEL )
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

    std::shared_ptr< IShaderBuilder > CreateShaderBuilder( const ParseTree & tree, const SymbolTable & symbolTable )
    {
        return std::make_shared< ShaderBuilder >( tree, symbolTable );
    }

    ShaderBuilder::ShaderBuilder( const ParseTree & tree, const SymbolTable & symbolTable )
        : mParseTree( tree )
    {
        AcquireValidator();

        // Build name-symbol map
        for ( auto & symbol : symbolTable )
        {
            mNameSymbolMap.insert( std::make_pair( symbol.name, &symbol ) );
        }

        mParseTree.TraverseDFS( *this, &ShaderBuilder::CollectVariables );

        // Build list of referenced texture-sampler pairs for each function
        {
            // Collect immediate references to sampling expressions
            for ( auto & symbol : symbolTable )
            {
                if ( symbol.isFunction )
                {
                    TextureSamplerPairSet & textureSamplerPairSet = mFunctionTextureSamplerMap[ &symbol ];
                    CollectTextureSampleExpressionsVisitor collectTextureSampleExpressions( mNameSymbolMap, textureSamplerPairSet );
                    symbol.tree->TraverseDFS( collectTextureSampleExpressions );
                }
            }

            // Collect indirect references to sampling expressions
            for ( auto & symbol : symbolTable )
            {
                if ( symbol.isFunction )
                {
                    TextureSamplerPairSet & functionTextureSamplerPairSet = mFunctionTextureSamplerMap[ &symbol ];
                    for ( auto & referencedSymbolIt : symbol.references )
                    {
                        if ( referencedSymbolIt.second->isFunction && referencedSymbolIt.second != &symbol )
                        {
                            TextureSamplerPairSet & referencedFunctionTextureSamplerPairSet =
                                mFunctionTextureSamplerMap[ referencedSymbolIt.second ];
                            functionTextureSamplerPairSet.insert( referencedFunctionTextureSamplerPairSet.begin(),
                                referencedFunctionTextureSamplerPairSet.end() );
                        }
                    }
                }
            }
        }
    }

    ShaderBuilder::~ShaderBuilder()
    {
        ReleaseValidator();
    }

    void ShaderBuilder::Build( ShaderBinary & shaderBinary, const std::string & shaderName, const std::string & profile )
    {
        // Query entry point's symbol

        auto entryPointIt = mNameSymbolMap.find( shaderName );
        CMN_THROW_IF( entryPointIt == mNameSymbolMap.end(),
            ShaderBuilderException::UndefinedEntry( shaderName.c_str() ) );

        const Symbol * entryPoint = entryPointIt->second;

        // Translate the profile

        ShaderStage stage;
        if ( profile == "vs_4_0" )
        {
            stage = SHADER_STAGE_VERTEX;
        }
        else if ( profile == "ps_4_0" )
        {
            stage = SHADER_STAGE_PIXEL;
        }
        else
        {
            stage = SHADER_STAGE_UNKNOWN;
        }

        // Assign registers

            typedef std::unordered_map< std::string, uint32_t > SymbolNameRegisterIndexMap;

            const Symbol * textureRegisterSymbolMap[ MAX_BIND_TEXTURES ];
            std::memset( textureRegisterSymbolMap, 0, sizeof( textureRegisterSymbolMap ) );
            SymbolNameRegisterIndexMap textureNameRegisterMap;

            const Symbol * samplerRegisterSymbolMap[ MAX_BIND_TEXTURES ];
            std::memset( samplerRegisterSymbolMap, 0, sizeof( samplerRegisterSymbolMap ) );
            SymbolNameRegisterIndexMap samplerNameRegisterMap;

            // Map explicitly assigned variables
            for ( auto it : entryPoint->references )
            {
                const Symbol & symbol = *it.second;
                if ( symbol.registerIndex != -1 )
                {
                    unsigned index = symbol.registerIndex;
                    if ( symbol.isTexture )
                    {
                        CMN_THROW_IF( index >= MAX_BIND_TEXTURES,
                            ShaderBuilderException::ExceededTextureRegistersLimit() );
                        CMN_THROW_IF( textureRegisterSymbolMap[ index ],
                            ShaderBuilderException::AssignedSameRegister( symbol.name,
                                textureRegisterSymbolMap[ index ]->name ) );
                        textureRegisterSymbolMap[ index ] = &symbol;
                        textureNameRegisterMap[ symbol.name ] = index;
                    }
                    else if ( symbol.isSamplerState )
                    {
                        CMN_THROW_IF( index >= MAX_BIND_TEXTURES,
                            ShaderBuilderException::ExceededSamplerRegistersLimit() );
                        CMN_THROW_IF( samplerRegisterSymbolMap[ index ],
                            ShaderBuilderException::AssignedSameRegister( symbol.name,
                                samplerRegisterSymbolMap[ index ]->name ) );
                        samplerRegisterSymbolMap[ index ] = &symbol;
                        samplerNameRegisterMap[ symbol.name ] = index;
                    }
                }
            }

            // Map not assigned variables
            for ( auto it : entryPoint->references )
            {
                const Symbol & symbol = *it.second;
                if ( symbol.registerIndex == -1 )
                {
                    int index = -1;
                    if ( symbol.isTexture )
                    {
                        for ( uint32_t i = 0; i < MAX_BIND_TEXTURES; ++ i )
                        {
                            if ( textureRegisterSymbolMap[ i ] == nullptr )
                            {
                                index = i;
                                break;
                            }
                        }
                        CMN_THROW_IF( index == -1, ShaderBuilderException::ExceededTextureRegistersLimit() );
                        textureRegisterSymbolMap[ index ] = &symbol;
                        textureNameRegisterMap[ symbol.name ] = index;
                    }
                    else if ( symbol.isSamplerState )
                    {
                        for ( uint32_t i = 0; i < MAX_BIND_TEXTURES; ++ i )
                        {
                            if ( samplerRegisterSymbolMap[ i ] == nullptr )
                            {
                                index = i;
                                break;
                            }
                        }
                        CMN_THROW_IF( index == -1, ShaderBuilderException::ExceededSamplerRegistersLimit() );
                        samplerRegisterSymbolMap[ index ] = &symbol;
                        samplerNameRegisterMap[ symbol.name ] = index;
                    }
                }
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
        ConstructSourceVisitor constructSource( source, textureSamplerPairSet, mNameSymbolMap, entryPoint );
        mParseTree.TraverseDFS( constructSource );
        source << std::endl << std::endl;

        // Declare built-in variables

        EnumInputsOutputs( entryPoint, ExpandInputOutputAsBuiltinDeclaration( source, mNameSymbolMap, stage ) );
        source << std::endl;

        // Declare inputs

        std::stringstream inputDecls;
        EnumInputsOutputs( entryPoint, ExpandInputAsGlobalDeclaration( inputDecls, mNameSymbolMap, stage ) );

        if ( stage == SHADER_STAGE_VERTEX )
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
        EnumInputsOutputs( entryPoint, ExpandOutputAsGlobalDeclaration( outputDecls, mNameSymbolMap, stage ) );

        if ( stage == SHADER_STAGE_PIXEL )
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

            EnumInputsOutputs( entryPoint, AssignInput( source, mNameSymbolMap, stage ) );

            // Call the entry point

            EnumInputsOutputs( entryPoint, ExpandInputOutputAsEntryPointArgument( source, entryPoint ) );

            // Assign outputs

            EnumInputsOutputs( entryPoint, AssignOutput( source, mNameSymbolMap, stage ) );
        }
        source << "}\n";

        Validate( stage, source.str().c_str() );

        // Construct binary

        shaderBinary.mDesc.inputsCount = entryPoint->args.size();
        shaderBinary.mStage = stage;

        ShaderBinaryOgl4 shaderBinaryOgl4;
        shaderBinaryOgl4.mSource = std::move( source.str() );

        for ( auto & textureSampler : mFunctionTextureSamplerMap[ entryPoint ] )
        {
            shaderBinaryOgl4.mTextureSamplerPairs.emplace_back(
                textureSampler.first->name,
                textureSampler.second->name );
        }

        std::ostringstream archiveStream;
        boost::archive::binary_oarchive archive( archiveStream );
        archive << shaderBinaryOgl4;

        shaderBinary.mData = std::move( archiveStream.str() );
    }

    bool ShaderBuilder::CollectVariables( const ParseTree & tree )
    {
        if ( tree.GetTokenType() == TOKEN_EXTERNAL_DECLARATION )
        {
            const ParseTree & child = tree.GetChild();
            if ( child.GetTokenType() == TOKEN_VARIABLE_DEFINITION ||
                 child.GetTokenType() == TOKEN_STATE_OBJECT_DEFINITION )
            {
                const ParseTree & name = child.GetFirstChildWithType( TOKEN_SYMBOL_NAME )->GetChild();
                mVariables[name.GetText()] = &child;
            }
            return false;
        }
        return true;
    }

} // namespace GFW
