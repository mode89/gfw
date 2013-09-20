#include "common/trace.h"

#include "gfw/base/device.h"
#include "gfw/core/input_layout.h"

#include "gfw/common/device_child.inl"

namespace GFW {

    InputLayout::InputLayout(uint32_t attrCnt, VertexAttribute attrs[], IShaderIn shader, IDeviceIn device)
        : ADeviceChild(device)
    {
        TRACE_ASSERT(shader.IsAttached());
    }

    InputLayout::~InputLayout()
    {

    }

} // namespace GFW
