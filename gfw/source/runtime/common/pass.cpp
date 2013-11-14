#include "common/trace.h"
#include "gfw/base/context.h"
#include "gfw/runtime/common/effect.h"
#include "gfw/runtime/common/pass.h"
#include "gfw/shared/pass.h"

#include "gfw/runtime/common/device_child.inl"

namespace GFW {

    Pass::Pass(  PassBinaryRef binary, const Effect * effect )
        : mEffect( effect )
    {
        for ( uint32_t stage = 0; stage < ShaderStage::COUNT; ++ stage )
        {
            uint32_t shaderIndex = binary->mShaders[stage];
            mShaders[stage] = mEffect->mShaders[shaderIndex];
        }
    }

    Pass::~Pass()
    {

    }

    void Pass::Dispatch()
    {
        IDeviceRef device = mEffect->GetDevice();
        IContextRef context = device->GetCurrentContext();
        TRACE_ASSERT( context.IsAttached() );

        for ( uint32_t stage = 0; stage< ShaderStage::COUNT; ++ stage )
        {
            context->SetShader( static_cast<ShaderStage>( stage ), mShaders[stage] );
        }
    }

} // namespace GFW
