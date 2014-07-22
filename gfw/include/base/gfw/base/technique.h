#ifndef __GFW_BASE_TECHNIQUE_H__
#define __GFW_BASE_TECHNIQUE_H__

#include "gfw/base/types_fwd.h"

namespace GFW {

    struct TechniqueDesc
    {
        uint32_t passCount;

        TechniqueDesc()
            : passCount( 0 )
        {}

        template < class Archive > void
        serialize( Archive & ar )
        {
            ar & passCount;
        }
    };

    class ITechnique
    {
    public:
        virtual const TechniqueDesc &
        GetDesc() const = 0;

        virtual void
        Dispatch( uint32_t pass = 0 ) const = 0;

        virtual ConstIShaderRef
        GetShader( ShaderStage, uint32_t pass = 0 ) const = 0;

        virtual
        ~ITechnique() {}
    };
    SHARED_PTR_TYPEDEFS( ITechnique );

} // namespace GFW

#endif // __GFW_BASE_TECHNIQUE_H__
