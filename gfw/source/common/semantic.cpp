#include "common/trace.h"

#include "gfw/common/semantic.h"

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

} // namespace GFW
