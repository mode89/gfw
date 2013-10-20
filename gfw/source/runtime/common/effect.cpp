#include "common/trace.h"

#include "gfw/base/context.h"

#include "gfw/common/effect.h"
#include "gfw/common/device_child.inl"

namespace GFW {

    Effect::Effect(IShaderRef shaders[], IDeviceIn device)
        : ADeviceChild(device)
    {
        for (int stage = 0; stage < SHADER_STAGE_COUNT; ++ stage)
        {
            TRACE_ASSERT(shaders[stage]->GetStage() == stage);
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

        for (int stage = 0; stage < SHADER_STAGE_COUNT; ++ stage)
        {
            context->SetShader(stage, mShaders[stage]);
        }
    }

} // namespace GFW
