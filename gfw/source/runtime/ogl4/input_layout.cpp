#include "common/trace.h"

#include "gfw/base/device.h"
#include "gfw/common/semantic.h"
#include "gfw/core/functions.h"
#include "gfw/core/input_layout.h"
#include "gfw/core/shader.h"
#include "gfw/core/shader_reflect.h"

#include "gfw/common/device_child.inl"

namespace GFW {

    InputLayout::InputLayout(uint32_t attrCnt, VertexAttribute attrs[], IShaderIn shader, IDeviceIn device)
        : ADeviceChild(device)
        , mAttributesMask(0)
    {
        TRACE_ASSERT(attrCnt <= MAX_INPUT_ATTRIBUTES);
        TRACE_ASSERT(shader.IsAttached());

        IShaderReflectionRef reflection = shader->GetReflection();

        for (uint32_t i = 0; i < attrCnt; ++ i)
        {
            VertexAttribute & attr = attrs[i];

            const ShaderParameterDesc & paramDesc = reflection->GetInputParameter(attr.semantic)->GetDesc();

            mAttributes[paramDesc.location] = attr;
            mAttributesMask |= (1 << paramDesc.location);
        }
    }

    InputLayout::~InputLayout()
    {

    }

} // namespace GFW
