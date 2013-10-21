#ifndef __GFW_PIPELINE_COMMON_PARSER_H__
#define __GFW_PIPELINE_COMMON_PARSER_H__

#include "common/autoref.h"

namespace GFW {

    struct ParserImpl;
    class  ParseTree;

    class Parser : public Common::ARefCounted
    {
    public:
        const ParseTree *
        GetTree() { return mTree; }

    public:
        Parser(const char * fileName);
        ~Parser();

    private:
        ParserImpl *    mImpl;
        ParseTree *     mTree;
    };
    AUTOREF_REFERENCE_DECLARATION( Parser );

} // namespace GFW

#endif // __GFW_PIPELINE_COMMON_PARSER_H__
