#ifndef __GFW_CORE_INPUT_LAYOUT_H__
#define __GFW_CORE_INPUT_LAYOUT_H__

#include "gfw/base/input_layout.h"
#include "gfw/base/vertex_attribute.h"
#include "gfw/runtime/common/device_child.h"
#include "gfw/runtime/core/limits.h"
#include "gfw/runtime/core/types_fwd.h"

namespace GFW {

    class InputLayout : public ADeviceChild<IInputLayout>
    {
    public:
        uint32_t
        GetEnabledAttributesMask() { return mAttributesMask; }

        const VertexAttribute &
        GetAttribute(uint32_t index) { return mAttributes[index]; }

    public:
        InputLayout(uint32_t attrCnt, VertexAttribute[], IShaderIn vertexShader, IDeviceIn);
        ~InputLayout();

    private:
        VertexAttribute     mAttributes[MAX_INPUT_ATTRIBUTES];
        uint32_t            mAttributesMask;
    };
    AUTOREF_REFERENCE_DECLARATION(InputLayout);

} // namespace GFW

#endif // __GFW_CORE_INPUT_LAYOUT_H__
