#ifndef __GFW_RUNTIME_COMMON_SEMANTIC_H__
#define __GFW_RUNTIME_COMMON_SEMANTIC_H__

#include "gfw/base/semantic.h"

namespace GFW {

    const char *
    GetSemanticString(Semantic);

    Semantic
    GetSemantic(const char *);

} // namespace GFW

#endif // __GFW_RUNTIME_COMMON_SEMANTIC_H__