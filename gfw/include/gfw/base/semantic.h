#ifndef __GFW_BASE_SEMANTIC_H__
#define __GFW_BASE_SEMANTIC_H__

namespace GFW {

#define BUILD_SEMANTIC(name, index) \
    SEMANTIC_ ## name ## index

#define BUILD_SEMANTIC_INDICES(semantic) \
    S(semantic, 0) \
    S(semantic, 1) \
    S(semantic, 2) \
    S(semantic, 3) \
    S(semantic, 4) \
    S(semantic, 5) \
    S(semantic, 6) \
    S(semantic, 7) \

#define SEMANTICS \
    BUILD_SEMANTIC_INDICES(POSITION) \
    BUILD_SEMANTIC_INDICES(TEXCOORD) \
    BUILD_SEMANTIC_INDICES(NORMAL) \
    BUILD_SEMANTIC_INDICES(COLOR) \

    enum Semantic
    {
        SEMANTIC_UNKNOWN = -1,
#define S(name, index) BUILD_SEMANTIC(name, index),
        SEMANTICS
#undef S
    };

} // namespace GFW

#endif // __GFW_BASE_SEMANTIC_H__
