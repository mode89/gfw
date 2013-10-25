#include "common/trace.h"
#include "gfw/pipeline/common/parse_tree.h"
#include "gfw/pipeline/shader_builder.h"

#include <string>

namespace GFW {

    class ConstructSourceVisitor
    {
    public:
        ConstructSourceVisitor( std::string & source )
            : mSource( source )
            , mLine( 1 )
            , mRow( 0 )
        {}

        bool operator() ( const ParseTree * tree )
        {
            if ( tree->ToString() )
            {
                uint32_t line = tree->GetLine();
                uint32_t row  = tree->GetRow();

                for ( ; mLine < line; ++ mLine )
                {
                    mSource += '\n';
                    mRow = 1;
                }

                for ( ; mRow < row; ++ mRow )
                {
                    mSource += ' ';
                }

                std::string token = tree->ToString();
                mSource += token;
                mRow += token.size();
            }

            return true;
        }

    private:
        std::string &   mSource;
        uint32_t        mLine;
        uint32_t        mRow;
    };

    ShaderBuilder::ShaderBuilder( const ParseTree * tree )
        : mParseTree( tree )
    {
        
    }

    ShaderBuilder::~ShaderBuilder()
    {

    }

    ShaderBinaryRef ShaderBuilder::Compile( const char * shaderName, const char * profile )
    {
        // Construct the shader's code
        std::string source;
        ConstructSourceVisitor constructSource( source );
        mParseTree->TraverseDFS( constructSource );

        return NULL;
    }

} // namespace GFW
