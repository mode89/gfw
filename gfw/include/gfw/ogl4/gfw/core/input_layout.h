#ifndef __GFW_CORE_INPUT_LAYOUT_H__
#define __GFW_CORE_INPUT_LAYOUT_H__

#include "gfw/base/input_layout.h"
#include "gfw/common/device_child.h"
#include "gfw/core/types_fwd.h"

namespace GFW {

    class InputLayout : public ADeviceChild<IInputLayout>
    {
    public:
        InputLayout(uint32_t attrCnt, VertexAttribute[], IShaderIn vertexShader, IDeviceIn);
        ~InputLayout();
    };
    AUTOREF_REFERENCE_DECLARATION(InputLayout);

} // namespace GFW

#endif // __GFW_CORE_INPUT_LAYOUT_H__
