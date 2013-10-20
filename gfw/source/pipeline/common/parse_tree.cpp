#include "common/trace.h"

#include "gfw/pipeline/common/parser.h"
#include "gfw/pipeline/common/parse_tree.h"

#include "FXLexer.h"
#include "FXParser.h"

namespace GFW { namespace Pipeline {

    static Token GetTokenType( uint32_t nativeType )
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
        , mType( TOKEN_UNKNOWN )
        , mChildren( NULL )
        , mChildCount( 0 )
    {
        mImpl = new ParseTreeImpl;

        mImpl->tree = static_cast<pANTLR3_BASE_TREE>( nativeTree );

        mType = GetTokenType( mImpl->tree->getType( mImpl->tree ) );

        mChildCount = mImpl->tree->getChildCount( mImpl->tree );
        mChildren = new ParseTree * [ mChildCount ];
        for (uint32_t i = 0; i < mChildCount; ++ i)
        {
            mChildren[i] = new ParseTree(mImpl->tree->getChild(mImpl->tree, i), mParser);
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

    bool ParseTree::TraverseDFS()
    {
        return true;
    }

}} // namespace GFW::Pipeline
