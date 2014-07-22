#ifndef __GFW_BASE_EFFECT_H__
#define __GFW_BASE_EFFECT_H__

#include "gfw/base/device_child.h"

namespace GFW {

    struct EffectDesc
    {
        uint32_t techniqueCount;

        EffectDesc()
            : techniqueCount( 0 )
        {}

        template < class Archive > void
        serialize( Archive & ar )
        {
            ar & techniqueCount;
        }
    };

    class IEffect
    {
    public:
        virtual const EffectDesc &
        GetDesc() const = 0;

        virtual void
        Dispatch( uint32_t tech = 0, uint32_t pass = 0 ) const = 0;

        virtual ConstITechniqueRef
        GetTechnique( const char * techName ) const = 0;

        virtual ConstIShaderRef
        GetShader( ShaderStage, uint32_t tech = 0, uint32_t pass = 0 ) const = 0;

        virtual
        ~IEffect() {}
    };
    SHARED_PTR_TYPEDEFS(IEffect);

} // namespace GFW

#endif // __GFW_BASE_EFFECT_H__
