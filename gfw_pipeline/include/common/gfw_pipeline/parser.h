#ifndef __GFW_PIPELINE_COMMON_PARSER_H__
#define __GFW_PIPELINE_COMMON_PARSER_H__

#include "common/autoref.h"

namespace GFW { namespace Pipeline {

    AUTOREF_FORWARD_DECLARATION( ParseTree );

    class Parser : public Common::ARefCounted
    {
    public:
        ParseTreeRef
        GetTree() { return mTree; }

    public:
        Parser(const char * fileName);
        ~Parser();

    private:
        void *          mImpl;
        ParseTree *     mTree;
    };
    AUTOREF_REFERENCE_DECLARATION( Parser );

}} // namespace GFW::Pipeline

#endif // __GFW_PIPELINE_COMMON_PARSER_H__
