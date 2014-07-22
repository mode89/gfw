#include "common/trace.h"
#include "gfw/pipeline/common/parse_tree.h"
#include "FXLexer.h"
#include "FXParser.h"

namespace GFW {

    static TokenType GetTokenType( uint32_t nativeType )
    {
        switch ( nativeType )
        {
#define T(token) case T_ ## token : return TOKEN_ ## token;
            TOKENS
#undef T
        default:
            break;
        }

        return TOKEN_UNKNOWN;
    }

    std::shared_ptr< const ParseTree > CreateParseTree( const boost::filesystem::path & filePath )
    {
        pANTLR3_UINT8               fName       = (pANTLR3_UINT8) filePath.c_str();
        pANTLR3_INPUT_STREAM        inputStream = NULL;
        pFXLexer                    lexer       = NULL;
        pANTLR3_COMMON_TOKEN_STREAM tokenStream = NULL;
        pFXParser                   parser      = NULL;
        std::shared_ptr< ParseTree > tree;

        inputStream = antlr3FileStreamNew( fName, ANTLR3_ENC_8BIT );
        if ( inputStream != NULL )
        {
            lexer = FXLexerNew( inputStream );
            if ( lexer != NULL )
            {
                tokenStream = antlr3CommonTokenStreamSourceNew( ANTLR3_SIZE_HINT, TOKENSOURCE( lexer ) );
                if ( tokenStream != NULL)
                {
                    parser = FXParserNew( tokenStream );
                    if ( parser != NULL )
                    {
                        FXParser_translation_unit_return ast = parser->translation_unit( parser );
                        if ( parser->pParser->rec->state->errorCount == 0 )
                        {
                            tree = std::make_shared<ParseTree>( ast.tree );
                        }
                    }
                    parser->free( parser );
                }
                lexer->free( lexer );
            }
            inputStream->close( inputStream );
        }

        TRACE_ASSERT( inputStream != NULL );
        TRACE_ASSERT( lexer != NULL );
        TRACE_ASSERT( tokenStream != NULL );
        TRACE_ASSERT( parser != NULL );
        TRACE_ASSERT( tree != NULL );

        return tree;
    }

    ParseTree::ParseTree( void * nativeTree )
        : mTokenType( TOKEN_UNKNOWN )
        , mLine( 0 )
        , mRow( 0 )
        , mEndLine( 0 )
        , mEndRow( 0 )
        , mChildCount( 0 )
    {
        pANTLR3_BASE_TREE tree = static_cast<pANTLR3_BASE_TREE>( nativeTree );

        pANTLR3_COMMON_TOKEN token = tree->getToken( tree );
        mTokenType = GFW::GetTokenType( token->getType( token ) );
        if ( token->getLine( token ) )
        {
            mString = reinterpret_cast< const char * >( token->getText( token )->chars );

            mLine = token->getLine( token );
            mEndLine = mLine;

            mRow = tree->getCharPositionInLine( tree );

            // FIXME Strange behavior of ANTLR
            if ( mLine == 1 && mRow > 0 )
            {
                mRow ++;
            }

            mEndRow = mRow + mString.size();
        }

        mChildCount = tree->getChildCount( tree );
        mChildren.reserve( mChildCount );
        for ( uint32_t i = 0; i < mChildCount; ++ i )
        {
            mChildren.emplace_back( tree->getChild( tree, i ) );
        }

        if ( mChildCount > 0 )
        {
            mLine = mChildren[0].GetLine();
            mRow  = mChildren[0].GetRow();

            mEndLine = mChildren.back().GetEndLine();
            mEndRow  = mChildren.back().GetEndRow();
        }
    }

    const ParseTree * ParseTree::GetFirstChildWithType( TokenType type ) const
    {
        for ( uint32_t i = 0; i < mChildCount; ++ i )
        {
            const ParseTree & child = mChildren[i];
            if ( child.GetTokenType() == type )
            {
                return &child;
            }
        }

        return nullptr;
    }

} // namespace GFW
