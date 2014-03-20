#ifndef __GFW_BASE_SEMANTIC_H__
#define __GFW_BASE_SEMANTIC_H__

namespace GFW {

#define BUILD_SEMANTIC(name, index) \
    SEMANTIC_ ## name ## index

#define BUILD_SEMANTIC_INDICES(semantic) \
    SI(semantic, 0) \
    SI(semantic, 1) \
    SI(semantic, 2) \
    SI(semantic, 3) \
    SI(semantic, 4) \
    SI(semantic, 5) \
    SI(semantic, 6) \
    SI(semantic, 7) \

#define SEMANTICS \
    S(POSITION) \
    S(TEXCOORD) \
    S(NORMAL) \
    S(COLOR) \

    enum Semantic
    {
        SEMANTIC_UNKNOWN = -1,
#define S(name) BUILD_SEMANTIC_INDICES( name )
#define SI(name, index) BUILD_SEMANTIC( name, index ),
        SEMANTICS
#undef SI
#undef S
    };

} // namespace GFW

#endif // __GFW_BASE_SEMANTIC_H__
