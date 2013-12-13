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

    ConstParseTreeRef CreateParseTree(  const char * fileName )
    {
        pANTLR3_UINT8 fName = (pANTLR3_UINT8) fileName;
        pANTLR3_INPUT_STREAM inputStream = antlr3FileStreamNew( fName, ANTLR3_ENC_8BIT );
        TRACE_ASSERT( inputStream != NULL );

        pFXLexer lexer = FXLexerNew( inputStream );
        TRACE_ASSERT( lexer != NULL );

        pANTLR3_COMMON_TOKEN_STREAM tokenStream = antlr3CommonTokenStreamSourceNew( ANTLR3_SIZE_HINT, TOKENSOURCE( lexer ) );
        TRACE_ASSERT( tokenStream != NULL);

        pFXParser parser = FXParserNew( tokenStream );
        TRACE_ASSERT( parser != NULL );

        FXParser_translation_unit_return ast = parser->translation_unit( parser );
        if ( parser->pParser->rec->state->errorCount > 0 )
        {
            TRACE_FAIL();

            // Free resources
            if ( parser ) parser->free( parser );
            if ( tokenStream ) tokenStream->free( tokenStream );
            if ( lexer ) lexer->free( lexer );
            if ( inputStream ) inputStream->close( inputStream );

            return NULL;
        }

        const ParseTree * tree = new ParseTree( ast.tree );

        // Free resources
        if ( parser ) parser->free( parser );
        if ( tokenStream ) tokenStream->free( tokenStream );
        if ( lexer ) lexer->free( lexer );
        if ( inputStream ) inputStream->close( inputStream );

        return tree;
    }

    ParseTree::ParseTree( void * nativeTree )
        : mTokenType( TOKEN_UNKNOWN )
        , mLine( 0 )
        , mRow( 0 )
        , mChildCount( 0 )
    {
        pANTLR3_BASE_TREE tree = static_cast<pANTLR3_BASE_TREE>( nativeTree );

        pANTLR3_COMMON_TOKEN token = tree->getToken( tree );
        mTokenType = GFW::GetTokenType( token->getType( token ) );
        mLine = token->getLine( token );
        if ( mLine )
        {
            mString = reinterpret_cast< const char * >( token->getText( token )->chars );
            mRow = tree->getCharPositionInLine( tree );
        }

        mChildCount = tree->getChildCount( tree );
        mChildren.resize( mChildCount );
        for ( uint32_t i = 0; i < mChildCount; ++ i )
        {
            mChildren[i] = new ParseTree( tree->getChild( tree, i ) );
        }
    }

    ConstParseTreeRef ParseTree::GetFirstChildWithType( TokenType type ) const
    {
        for ( uint32_t i = 0; i < mChildCount; ++ i )
        {
            const ParseTreeRef child = mChildren[i];
            if ( child->GetTokenType() == type )
            {
                return child;
            }
        }

        return NULL;
    }

} // namespace GFW
