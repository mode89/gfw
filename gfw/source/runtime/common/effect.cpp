#include "common/trace.h"

#include "gfw/base/context.h"

#include "gfw/runtime/common/effect.h"
#include "gfw/runtime/common/device_child.inl"

namespace GFW {

    Effect::Effect(IShaderRef shaders[], IDeviceIn device)
        : ADeviceChild(device)
    {
        for ( uint32_t stage = 0; stage < ShaderStage::COUNT; ++ stage )
        {
            TRACE_ASSERT( shaders[stage]->GetStage() == stage );
            mShaders[stage] = shaders[stage];
        }
    }

    Effect::~Effect()
    {

    }

    void Effect::Dispatch()
    {
        IContextRef context = mDevice->GetCurrentContext();
        TRACE_ASSERT(context.IsAttached());

        for (int stage = 0; stage < ShaderStage::COUNT; ++ stage)
        {
            context->SetShader( static_cast<ShaderStage>( stage ), mShaders[stage]);
        }
    }

} // namespace GFW
