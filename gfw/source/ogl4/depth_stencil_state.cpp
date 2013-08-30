#include "common/trace.h"

#include "gfw/common/device_child.inl"

#include "gfw/core/depth_stencil_state.h"
#include "gfw/core/functions.h"

namespace GFW {

    static uint32_t DepthStencilState_GetCompareFunc(CompareFunc func)
    {
        switch(func)
        {
        case COMPARE_NEVER:             return GL_NEVER;
        case COMPARE_LESS:              return GL_LESS;
        case COMPARE_EQUAL:             return GL_EQUAL;
        case COMPARE_LESS_EQUAL:        return GL_LEQUAL;
        case COMPARE_GREATER:           return GL_GREATER;
        case COMPARE_NOT_EQUAL:         return GL_NOTEQUAL;
        case COMPARE_GREATER_EQUAL:     return GL_GEQUAL;
        case COMPARE_ALWAYS:            return GL_ALWAYS;
        default:
            TRACE_FAIL();
        }

        return GL_LEQUAL;
    }

    static uint32_t DepthStencilState_GetStencilOp(StencilOp op)
    {
        switch (op)
        {
            case STENCIL_OP_KEEP:       return GL_KEEP;
            case STENCIL_OP_ZERO:       return GL_ZERO;
            case STENCIL_OP_REPLACE:    return GL_REPLACE;
            case STENCIL_OP_INCR_SAT:   return GL_INCR_WRAP;
            case STENCIL_OP_DECR_SAT:   return GL_DECR_WRAP;
            case STENCIL_OP_INVERT:     return GL_INVERT;
            case STENCIL_OP_INCR:       return GL_INCR;
            case STENCIL_OP_DECR:       return GL_DECR;
            default:
                TRACE_FAIL();
        }

        return GL_KEEP;
    }

    DepthStencilState::DepthStencilState(const DepthStencilStateDesc & desc, uint32_t descHash, IDeviceRef device)
        : ADeviceChild(device)
        , mDesc(desc)
    { }

    DepthStencilState::~DepthStencilState()
    { }

    void DepthStencilState::Apply() const
    {
        if (mDesc.stencilEnable)
        {
            TRACE_ASSERT_GL(glEnable, GL_STENCIL_TEST);

            uint32_t frontStencilFailOp      = DepthStencilState_GetStencilOp(mDesc.frontFace.stencilFailOp);
            uint32_t frontStencilDepthFailOp = DepthStencilState_GetStencilOp(mDesc.frontFace.stencilDepthFailOp);
            uint32_t frontStencilPassOp      = DepthStencilState_GetStencilOp(mDesc.frontFace.stencilPassOp);
            uint32_t frontStencilFunc        = DepthStencilState_GetCompareFunc(mDesc.frontFace.stencilFunc);

            uint32_t backStencilFailOp       = DepthStencilState_GetStencilOp(mDesc.backFace.stencilFailOp);
            uint32_t backStencilDepthFailOp  = DepthStencilState_GetStencilOp(mDesc.backFace.stencilDepthFailOp);
            uint32_t backStencilPassOp       = DepthStencilState_GetStencilOp(mDesc.backFace.stencilPassOp);
            uint32_t backStencilFunc         = DepthStencilState_GetCompareFunc(mDesc.backFace.stencilFunc);

            uint32_t stencilOpSeparation = 0;

            stencilOpSeparation = stencilOpSeparation || (frontStencilFailOp      != backStencilFailOp);
            stencilOpSeparation = stencilOpSeparation || (frontStencilDepthFailOp != backStencilDepthFailOp);
            stencilOpSeparation = stencilOpSeparation || (frontStencilPassOp      != backStencilPassOp);

            if (stencilOpSeparation)
            {
                TRACE_ASSERT_GL(glStencilOpSeparate, GL_FRONT, frontStencilFailOp, frontStencilDepthFailOp, frontStencilPassOp);
                TRACE_ASSERT_GL(glStencilOpSeparate, GL_BACK , backStencilFailOp , backStencilDepthFailOp , backStencilPassOp);
            }
            else
            {
                TRACE_ASSERT_GL(glStencilOp, frontStencilFailOp, frontStencilDepthFailOp, frontStencilPassOp);
            }

            uint32_t stencilFuncSeparation = 0;

            stencilFuncSeparation = stencilFuncSeparation || (frontStencilFunc != backStencilFunc);
            stencilFuncSeparation = stencilFuncSeparation || (mDesc.frontFace.stencilRef != mDesc.backFace.stencilRef);
            stencilFuncSeparation = stencilFuncSeparation || (mDesc.frontFace.stencilReadMask != mDesc.backFace.stencilReadMask);

            // Note:
            // we use only stencilReadMask, since in paragraph 17.3.5 of OpenGL 4.3 (Core Proﬁle, August 6, 2012) spec we can find the followinf statement:
            // The s least signiﬁcant bits of mask are bitwise ANDed with both the reference and the stored stencil value,
            // and the resulting masked values are those that participate in the comparison controlled by 'func'
            if (stencilFuncSeparation)
            {
                TRACE_ASSERT_GL(glStencilFuncSeparate, GL_FRONT, frontStencilFunc, mDesc.frontFace.stencilRef, mDesc.frontFace.stencilReadMask);
                TRACE_ASSERT_GL(glStencilFuncSeparate, GL_BACK , backStencilFunc , mDesc.backFace.stencilRef , mDesc.backFace.stencilReadMask);
            }
            else
            {
                TRACE_ASSERT_GL(glStencilFunc, frontStencilFunc, mDesc.backFace.stencilRef, mDesc.backFace.stencilReadMask);
            }

            if (mDesc.frontFace.stencilReadMask != mDesc.backFace.stencilWriteMask)
            {
                TRACE_ASSERT_GL(glStencilMaskSeparate, GL_FRONT, mDesc.frontFace.stencilWriteMask);
                TRACE_ASSERT_GL(glStencilMaskSeparate, GL_BACK , mDesc.backFace.stencilWriteMask);
            }
            else
            {
                TRACE_ASSERT_GL(glStencilMask, mDesc.backFace.stencilWriteMask);
            }
        }
        else
        {
            TRACE_ASSERT_GL(glDisable, GL_STENCIL_TEST);
        }

        if (mDesc.depthEnable)
        {
            uint32_t depthFunc = DepthStencilState_GetCompareFunc(mDesc.depthFunc);

            TRACE_ASSERT_GL(glEnable, GL_DEPTH_TEST);
            TRACE_ASSERT_GL(glDepthFunc, depthFunc);
            TRACE_ASSERT_GL(glDepthMask, mDesc.depthWriteEnable);
        }
        else
        {
            TRACE_ASSERT_GL(glDisable, GL_DEPTH_TEST);
        }
    }

} // namespace GFW
