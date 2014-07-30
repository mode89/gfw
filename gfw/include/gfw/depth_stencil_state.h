#ifndef __GFW_BASE_DEPTH_STENCIL_STATE_H__
#define __GFW_BASE_DEPTH_STENCIL_STATE_H__

#include "gfw/compare.h"
#include "gfw/device_child.h"
#include "gfw/types_fwd.h"

namespace GFW {

    /// The stencil operations that can be performed during depth-stencil testing.
    enum StencilOp
    {
        STENCIL_OP_KEEP,         ///< Keep the existing stencil data.
        STENCIL_OP_ZERO,         ///< Set the stencil data to 0.
        STENCIL_OP_REPLACE,      ///< Set the stencil data to the reference value.
        STENCIL_OP_INCR_SAT,     ///< Increment the stencil value by 1, and clamp the result.
        STENCIL_OP_DECR_SAT,     ///< Decrement the stencil value by 1, and clamp the result.
        STENCIL_OP_INVERT,       ///< Invert the stencil data.
        STENCIL_OP_INCR,         ///< Increment the stencil value by 1, and wrap the result if necessary.
        STENCIL_OP_DECR,         ///< Decrement the stencil value by 1, and wrap the result if necessary.
    };

    /// Describes stencil operations to be performed based on the results of stencil test.
    struct StencilOpDesc
    {
        StencilOp       stencilFailOp;      ///< The stencil operation to perform when stencil testing fails.
        StencilOp       stencilDepthFailOp; ///< The stencil operation to perform when stencil testing passes and depth testing fails.
        StencilOp       stencilPassOp;      ///< The stencil operation to perform when stencil testing and depth testing both pass.
        CompareFunc     stencilFunc;        ///< A function that compares stencil data against a reference value.
        uint32_t        stencilRef;         ///< The stencil reference value.
        uint32_t        stencilReadMask;    ///< Identify a portion of the depth-stencil buffer for reading stencil data.
        uint32_t        stencilWriteMask;   ///< Identify a portion of the depth-stencil buffer for writing stencil data.

        StencilOpDesc()
            : stencilFailOp(STENCIL_OP_KEEP)
            , stencilDepthFailOp(STENCIL_OP_KEEP)
            , stencilPassOp(STENCIL_OP_KEEP)
            , stencilFunc(COMPARE_ALWAYS)
            , stencilRef(0u)
            , stencilReadMask(0xFFFFFFFF)
            , stencilWriteMask(0xFFFFFFFF)
        {}
    };

    /// Describes depth-stencil state.
    struct DepthStencilStateDesc
    {
        StencilOpDesc       frontFace;          ///< Identify how to use the results of the depth test and the stencil test for pixels whose surface normal is facing towards the camera.
        StencilOpDesc       backFace;           ///< Identify how to use the results of the depth test and the stencil test for pixels whose surface normal is facing away from the camera.
        CompareFunc         depthFunc;          ///< A function that compares depth data against existing depth data.
        bool                depthEnable;        ///< Enable depth testing.
        bool                depthWriteEnable;   ///< Enable writes to the depth portion of the depth-stencil buffer.
        bool                stencilEnable;      ///< Enable stencil testing.

        DepthStencilStateDesc()
            : depthFunc(COMPARE_LESS_EQUAL)
            , depthEnable(true)
            , depthWriteEnable(true)
            , stencilEnable(false)
        {}
    };

    class IDepthStencilState : public IDeviceChild
    {
    public:
        virtual const DepthStencilStateDesc &
        GetDesc() = 0;

        virtual void
        Apply() const = 0;

        virtual
        ~IDepthStencilState() {}
    };

} // namespace GFW

#endif // __GFW_BASE_DEPTH_STENCIL_STATE_H__
