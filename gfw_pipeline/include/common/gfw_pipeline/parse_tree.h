#ifndef __GFW_PIPELINE_COMMON_PARSE_TREE_H__
#define __GFW_PIPELINE_COMMON_PARSE_TREE_H__

#include "common/autoref.h"

namespace GFW { namespace Pipeline {

#define TOKENS \
    T(TRANSLATION_UNIT) \
    T(EXTERNAL_DECLARATION) \
    T(FUNCTION_DEFINITION) \
    T(TECHNIQUE_DEFINITION) \
    T(PASS_DEFINITION) \
    T(ARGUMENTS_LIST) \
    T(ARGUMENT) \
    T(SEMANTIC) \
    T(SET_SHADER) \
    T(ID) \

    enum Token
    {
        TOKEN_UNKNOWN = 0,
#define T(token) TOKEN_ ## token,
        TOKENS
#undef T
    };

    struct ParseTreeImpl;
    AUTOREF_FORWARD_DECLARATION( Parser );

    class ParseTree
    {
    public:
        bool
        TraverseDFS();

    public:
        ParseTree( void * nativeTree, ParserIn );
        ~ParseTree();

    private:
        ParseTreeImpl * mImpl;
        ParserRef       mParser;

        Token           mType;

        ParseTree **    mChildren;
        uint32_t        mChildCount;
    };

}} // namespace GFW::Pipeline

#endif // __GFW_PIPELINE_COMMON_PARSE_TREE_H__
