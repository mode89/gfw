#include "common/trace.h"

#include "gfw/common/semantic.h"

#include <string.h>

namespace GFW {

    const char * GetSemanticString(Semantic semantic)
    {
        switch (semantic)
        {
#define S(name, index) case BUILD_SEMANTIC(name, index) : return # name # index ;
            SEMANTICS
#undef S
        default:
            TRACE_FAIL_MSG("Unknown semantic");
        }

        return NULL;
    }

    Semantic GetSemantic(const char * str)
    {
#define S(name, index) if (strcmp(str, # name # index) == 0) return BUILD_SEMANTIC(name, index);
        SEMANTICS
#undef S

        TRACE_FAIL_MSG("Unknown semantic");
        return SEMANTIC_UNKNOWN;
    }

} // namespace GFW
