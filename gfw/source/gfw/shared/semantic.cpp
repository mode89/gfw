#include "cmn/trace.h"
#include "gfw/semantic.h"
#include <cstring>

namespace GFW {

    const char * GetSemanticString(Semantic semantic)
    {
        switch (semantic)
        {
#define S( name ) BUILD_SEMANTIC_INDICES( name )
#define SI( name, index ) case BUILD_SEMANTIC( name, index ) : return # name # index ;
            SEMANTICS
#undef SI
#undef S
        default:
            CMN_FAIL_MSG( "Unknown semantic" );
        }
        return NULL;
    }

    Semantic GetSemantic(const char * str)
    {
        // Parse semantics without tailing index
#define S( name ) if ( std::strcmp( str, # name ) == 0 ) return BUILD_SEMANTIC( name, 0 );
        SEMANTICS
#undef S
        // Parse semanitcs with tailing index
#define S( name ) BUILD_SEMANTIC_INDICES( name )
#define SI( name, index ) if ( std::strcmp( str, # name # index ) == 0 ) return BUILD_SEMANTIC( name, index );
        SEMANTICS
#undef SI
#undef S
        return SEMANTIC_UNKNOWN;
    }

} // namespace GFW
