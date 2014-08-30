#ifndef __GFW_RUNTIME_CORE_INPUT_LAYOUT_H__
#define __GFW_RUNTIME_CORE_INPUT_LAYOUT_H__

#include "gfw/input_layout.h"
#include "gfw/vertex_attribute.h"
#include "gfw/runtime/common/device_child.h"
#include "gfw/runtime/ogl4/limits.h"
#include "gfw/runtime/ogl4/types_fwd.h"

namespace GFW {

    class InputLayout : public ADeviceChild<IInputLayout>
    {
    public:
        uint32_t
        GetEnabledAttributesMask() const { return mAttributesMask; }

        const VertexAttribute &
        GetAttribute(uint32_t index) const { return mAttributes[index]; }

        size_t
        GetInputSignature() const { return mInputSignature; }

    public:
        InputLayout(uint32_t attrCnt, VertexAttribute[], ConstIShaderIn vertexShader, DeviceIn);
        ~InputLayout();

    private:
        VertexAttribute     mAttributes[MAX_INPUT_ATTRIBUTES];
        unsigned            mAttributesMask;
        size_t              mInputSignature;
    };
    SHARED_PTR_TYPEDEFS(InputLayout);

} // namespace GFW

#endif // __GFW_RUNTIME_CORE_INPUT_LAYOUT_H__
