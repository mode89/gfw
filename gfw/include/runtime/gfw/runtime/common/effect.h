#ifndef __GFW_RUNTIME_COMMON_EFFECT_H__
#define __GFW_RUNTIME_COMMON_EFFECT_H__

#include "gfw/base/effect.h"
#include "gfw/runtime/common/shader_table.h"
#include "gfw/runtime/common/types_fwd.h"
#include "gfw/shared/types_fwd.h"

#include <string>
#include <unordered_map>
#include <vector>

namespace GFW {

    class Effect : public IEffect
    {
    public:
        virtual const EffectDesc &
        GetDesc() const { return mDesc; }

        virtual void
        Dispatch( uint32_t tech, uint32_t pass ) const;

        virtual ConstITechniqueRef
        GetTechnique( const char * techName ) const;

        virtual ConstIShaderRef
        GetShader( ShaderStage stage, uint32_t tech, uint32_t pass ) const;

    public:
        Effect( const EffectBinary &, IDeviceIn );
        ~Effect();

    private:
        typedef std::vector< TechniqueRef > TechniqueVec;
        typedef std::unordered_map< std::string, TechniqueRef > TechniqueMap;

        EffectDesc      mDesc;

        TechniqueVec    mTechniques;
        ShaderTable     mShaderTable;

        TechniqueMap    mTechniqueMap;
    };
    SHARED_PTR_TYPEDEFS(Effect);

} // namespace GFW

#endif // __GFW_RUNTIME_COMMON_EFFECT_H__
