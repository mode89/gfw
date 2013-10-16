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
        ParseTreeImpl * imp = new ParseTreeImpl;
        mImpl = imp;

        imp->tree = static_cast<pANTLR3_BASE_TREE>( nativeTree );
    }

    ParseTree::~ParseTree()
    {
        if ( mImpl != NULL )
        {
            ParseTreeImpl * imp = static_cast<ParseTreeImpl*>( mImpl );
            delete imp;
        }
    }

    void ParseTree::TraverseDFS()
    {
        ParseTreeImpl * imp = static_cast<ParseTreeImpl*>( mImpl );
    }

}} // namespace GFW::Pipeline
