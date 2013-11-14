#ifndef __GFW_COMMON_PASS_H__
#define __GFW_COMMON_PASS_H__

#include "gfw/base/pass.h"
#include "gfw/base/shader.h"
#include "gfw/shared/shader_stage.h"

namespace GFW {

    class Effect;

    class Pass : public IPass
    {
    public:
        virtual void
        Dispatch();

        virtual IShaderRef
        GetShader( ShaderStage stage ) const { return mShaders[stage]; }

    public:
        Pass( PassBinaryRef, const Effect * );
        ~Pass();

    private:
        const Effect *  mEffect;
        IShaderRef      mShaders[ ShaderStage::COUNT ];
    };
    AUTOREF_REFERENCE_DECLARATION( Pass );

} // namespace GFW

#endif // __GFW_COMMON_PASS_H__
