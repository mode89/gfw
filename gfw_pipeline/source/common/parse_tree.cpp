#include "common/trace.h"

#include "gfw_pipeline/parser.h"
#include "gfw_pipeline/parse_tree.h"

#include "FXLexer.h"
#include "FXParser.h"

namespace GFW { namespace Pipeline {

    struct ParseTreeImpl
    {
        pANTLR3_BASE_TREE   tree;
    };

    ParseTree::ParseTree( void * nativeTree, ParserIn parser )
        : mImpl( NULL )
        , mParser( parser )
    {
        mImpl = new ParseTreeImpl;

        mImpl->tree = static_cast<pANTLR3_BASE_TREE>( nativeTree );

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

    void ParseTree::TraverseDFS()
    {

    }

}} // namespace GFW::Pipeline
