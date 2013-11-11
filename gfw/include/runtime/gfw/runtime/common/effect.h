#ifndef __GFW_RUNTIME_COMMON_EFFECT_H__
#define __GFW_RUNTIME_COMMON_EFFECT_H__

#include "gfw/base/effect.h"
#include "gfw/base/shader.h"
#include "gfw/base/types_fwd.h"
#include "gfw/runtime/common/device_child.h"
#include "gfw/shared/effect.h"

#include <string>
#include <unordered_map>
#include <vector>

namespace GFW {

    class Effect : public ADeviceChild<IEffect>
    {
    public:
        virtual void
        Dispatch( uint32_t tech = 0, uint32_t pass = 0 );

        virtual ITechniqueRef
        GetTechnique( const char * techName );

        virtual IShaderRef
        GetShader( ShaderStage stage, uint32_t tech = 0, uint32_t pass = 0 );

    public:
        Effect( EffectBinaryRef, IDeviceIn );
        ~Effect();

    private:
        typedef std::vector< ITechniqueRef > TechniqueVec;
        typedef std::vector< IShaderRef > ShaderVec;
        typedef std::unordered_map< std::string, ITechniqueRef > TechniqueMap;

        EffectDesc      mDesc;

        TechniqueVec    mTechniques;
        ShaderVec       mShaders;

        TechniqueMap    mTechniqueMap;
    };
    AUTOREF_REFERENCE_DECLARATION(Effect);

} // namespace GFW

#endif // __GFW_RUNTIME_COMMON_EFFECT_H__
