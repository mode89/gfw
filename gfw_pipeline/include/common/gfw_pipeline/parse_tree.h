#ifndef __GFW_PIPELINE_COMMON_PARSE_TREE_H__
#define __GFW_PIPELINE_COMMON_PARSE_TREE_H__

#include "common/autoref.h"

namespace GFW { namespace Pipeline {

    struct ParseTreeImpl;
    AUTOREF_FORWARD_DECLARATION( Parser );

    class ParseTree : public Common::ARefCounted
    {
    public:
        void
        TraverseDFS();

    public:
        ParseTree( void * nativeTree, ParserIn );
        ~ParseTree();

    private:
        ParseTreeImpl * mImpl;
        ParserRef       mParser;

        ParseTree **    mChildren;
        uint32_t        mChildCount;
    };
    AUTOREF_REFERENCE_DECLARATION( ParseTree );

}} // namespace GFW::Pipeline

#endif // __GFW_PIPELINE_COMMON_PARSE_TREE_H__
