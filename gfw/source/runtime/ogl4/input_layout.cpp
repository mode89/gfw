#include "cmn/trace.h"

#include "gfw/base/device.h"
#include "gfw/runtime/common/semantic.h"
#include "gfw/runtime/core/device.h"
#include "gfw/runtime/core/functions.h"
#include "gfw/runtime/core/input_layout.h"
#include "gfw/runtime/core/shader.h"
#include "gfw/runtime/core/shader_reflect.h"

#include "gfw/runtime/common/device_child.inl"

namespace GFW {

    InputLayout::InputLayout( uint32_t attrCnt, VertexAttribute attrs[], ConstIShaderIn shader, DeviceIn device )
        : ADeviceChild( device )
        , mAttributesMask( 0 )
    {
        TRACE_ASSERT(attrCnt <= MAX_INPUT_ATTRIBUTES);
        TRACE_ASSERT(shader);

        ConstIShaderReflectionRef reflection = shader->GetReflection();

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
