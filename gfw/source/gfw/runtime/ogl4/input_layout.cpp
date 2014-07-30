#include "cmn/trace.h"

#include "gfw/device.h"
#include "gfw/runtime/common/semantic.h"
#include "gfw/runtime/ogl4/device.h"
#include "gfw/runtime/ogl4/functions.h"
#include "gfw/runtime/ogl4/input_layout.h"
#include "gfw/runtime/ogl4/shader.h"
#include "gfw/runtime/ogl4/shader_reflect.h"

#include "gfw/runtime/common/device_child.inl"

namespace GFW {

    InputLayout::InputLayout( uint32_t attrCnt, VertexAttribute attrs[], ConstIShaderIn shader, DeviceIn device )
        : ADeviceChild( device )
        , mAttributesMask( 0 )
    {
        CMN_ASSERT( attrCnt <= MAX_INPUT_ATTRIBUTES );
        CMN_ASSERT( shader );

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
