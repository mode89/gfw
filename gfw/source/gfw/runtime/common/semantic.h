#ifndef __GFW_SHARED_SEMANTIC_H__
#define __GFW_SHARED_SEMANTIC_H__

#include "gfw/semantic.h"

namespace GFW {

    const char *
    GetSemanticString(Semantic);

    Semantic
    GetSemantic(const char *);

} // namespace GFW

#endif // __GFW_SHARED_SEMANTIC_H__
