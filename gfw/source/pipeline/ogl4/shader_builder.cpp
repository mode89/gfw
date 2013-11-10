#include "common/trace.h"
#include "gfw/pipeline/common/parse_tree.h"
#include "gfw/pipeline/shader_builder.h"

#include <cstring>
#include <string>
#include <sstream>

namespace GFW {

    class ConstructSourceVisitor
    {
    public:
        ConstructSourceVisitor( std::stringstream & source )
            : mSource( source )
            , mLine( 1 )
            , mRow( 0 )
        {}

        bool operator() ( const ParseTree * tree )
        {
            TokenType tokenType = tree->GetTokenType();

            // Skip FX-specific stuff
            if ( tokenType == TOKEN_TECHNIQUE_DEFINITION ||
                 tokenType == TOKEN_SEMANTIC )
            {
                return false;
            }

            if ( tree->ToString() )
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

                std::string token = tree->ToString();
                mSource << token;
                mRow += token.size();
            }

            return true;
        }

    private:
        std::stringstream & mSource;
        uint32_t            mLine;
        uint32_t            mRow;
    };

    ShaderBuilder::ShaderBuilder( const ParseTree * tree )
        : mParseTree( tree )
    {
        mParseTree->TraverseDFS( *this, &ShaderBuilder::CollectFunctions );
    }

    ShaderBuilder::~ShaderBuilder()
    {

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

        ConstructSourceVisitor constructSource( source );
        mParseTree->TraverseDFS( constructSource );

        source << std::endl << std::endl;

        FunctionMap::iterator it = mFunctions.find( shaderName );
        TRACE_ASSERT( it != mFunctions.end() );
        Function & entryPoint = it->second;

        // Input parameters

        for ( uint32_t i = 0; i < entryPoint.args.size(); ++ i )
        {
            const ParseTree * arg = entryPoint.args[i];
            source << "in ";
            source << arg->GetChild( 0 )->ToString();
            source << " _in_";
            source << i;
            source << "; // ";
            source << arg->GetChild( 1 )->ToString();

            const ParseTree * semantic = arg->GetFirstChildWithType( TOKEN_SEMANTIC );
            if ( semantic != NULL )
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
                if ( entryPoint.sem != NULL )
                {
                    source << "    ";
                    const ParseTree * semantic = entryPoint.sem->GetChild();
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

        // Construct binary

        ShaderBinaryRef shaderBinary = new ShaderBinary;

        shaderBinary->mDesc.stage       = stage;
        shaderBinary->mDesc.inputsCount = entryPoint.args.size();

        uint32_t sourceSize = source.str().size() + 1; // + 1 for null-terminator
        shaderBinary->mSize = sourceSize;
        shaderBinary->mData = new uint8_t [ sourceSize ];
        std::memcpy( shaderBinary->mData, source.str().c_str(), sourceSize );

        return shaderBinary;
    }

    bool ShaderBuilder::CollectFunctions( const ParseTree * tree )
    {
        if ( tree->GetTokenType() == TOKEN_EXTERNAL_DECLARATION )
        {
            const ParseTree * child = tree->GetChild();
            if ( child->GetTokenType() == TOKEN_FUNCTION_DEFINITION )
            {
                const ParseTree * name = child->GetFirstChildWithType( TOKEN_ID );

                Function & func = mFunctions[ name->ToString() ];

                func.tree = child;
                func.ret  = child->GetChild();
                func.sem  = child->GetFirstChildWithType( TOKEN_SEMANTIC );

                // Collect arguments

                const ParseTree * args = child->GetFirstChildWithType( TOKEN_ARGUMENTS_LIST );
                if ( args != NULL )
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

} // namespace GFW
