#include "common/trace.h"

#include "gfw/pipeline/common/parser.h"
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

    struct ParseTreeImpl
    {
        pANTLR3_BASE_TREE   tree;
    };

    ParseTree::ParseTree( void * nativeTree, ParserIn parser )
        : mImpl( NULL )
        , mParser( parser )
        , mString( NULL )
        , mTokenType( TOKEN_UNKNOWN )
        , mLine( 0 )
        , mRow( 0 )
        , mChildren( NULL )
        , mChildCount( 0 )
    {
        mImpl = new ParseTreeImpl;

        mImpl->tree = static_cast<pANTLR3_BASE_TREE>( nativeTree );
        pANTLR3_BASE_TREE tree = mImpl->tree;

        pANTLR3_COMMON_TOKEN token = tree->getToken( tree );
        mTokenType = GFW::GetTokenType( token->getType( token ) );
        mLine = token->getLine( token );
        if ( mLine )
        {
            mString = reinterpret_cast< const char * >( token->getText( token )->chars );
            mRow = tree->getCharPositionInLine( tree );
        }

        mChildCount = tree->getChildCount( tree );
        mChildren = new ParseTree * [ mChildCount ];
        for ( uint32_t i = 0; i < mChildCount; ++ i )
        {
            mChildren[i] = new ParseTree( tree->getChild( tree, i ), mParser );
        }
    }

    ParseTree::~ParseTree()
    {
        if ( mChildren != NULL )
        {
            for (uint32_t i = 0; i < mChildCount; ++ i)
            {
                delete mChildren[i];
            }
            delete [] mChildren;
        }

        if ( mImpl != NULL )
        {
            delete mImpl;
        }
    }

} // namespace GFW
