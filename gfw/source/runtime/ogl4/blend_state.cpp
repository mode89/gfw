#include "cmn/trace.h"
#include "gfw/runtime/common/device_child.inl"
#include "gfw/runtime/core/blend_state.h"
#include "gfw/runtime/core/device.h"
#include "gfw/runtime/core/functions.h"

namespace GFW {

    static uint32_t BlendState_GetBlendFactor(BlendFactor blendFactor)
    {
        switch(blendFactor)
        {
        case BLEND_FACTOR_ZERO:             return GL_ZERO;
        case BLEND_FACTOR_ONE:              return GL_ONE;
        case BLEND_FACTOR_SRC_COLOR:        return GL_SRC_COLOR;
        case BLEND_FACTOR_INV_SRC_COLOR:    return GL_ONE_MINUS_SRC_COLOR;
        case BLEND_FACTOR_SRC_ALPHA:        return GL_SRC_ALPHA;
        case BLEND_FACTOR_INV_SRC_ALPHA:    return GL_ONE_MINUS_SRC_ALPHA;
        case BLEND_FACTOR_DEST_ALPHA:       return GL_DST_ALPHA;
        case BLEND_FACTOR_INV_DEST_ALPHA:   return GL_ONE_MINUS_DST_ALPHA;
        case BLEND_FACTOR_DEST_COLOR:       return GL_DST_COLOR;
        case BLEND_FACTOR_INV_DEST_COLOR:   return GL_ONE_MINUS_DST_COLOR;
        case BLEND_FACTOR_SRC_ALPHA_SAT:    return GL_SRC_ALPHA_SATURATE;
        default:
            TRACE_FAIL();
        }

        return GL_ZERO;
    }

    static uint32_t BlendState_GetBlendOp(BlendOp blendOp)
    {
        switch (blendOp)
        {
        case BLEND_OP_ADD:                  return GL_FUNC_ADD;
        case BLEND_OP_SUBTRACT:             return GL_FUNC_SUBTRACT;        
        case BLEND_OP_REVERSE_SUBTRACT:     return GL_FUNC_REVERSE_SUBTRACT;
        case BLEND_OP_MIN:                  return GL_MIN;
        case BLEND_OP_MAX:                  return GL_MAX;
        default:
            TRACE_FAIL();
        }

        return GL_FUNC_ADD;
    }

    BlendState::BlendState(const BlendStateDesc & desc, uint32_t descHash, DeviceIn device)
        : ADeviceChild(device)
        , mDesc(desc)
    {

    }

    BlendState::~BlendState()
    {

    }

    void BlendState::Apply() const
    {
        // Disable blend foreach ignored target
        if ( !mDesc.independentBlendEnable)
        {
            for (uint32_t i = 1; i < 8; ++i)
            {
                VGL( glDisablei, GL_BLEND, i );
            }
        }

        const uint32_t rtCount = mDesc.independentBlendEnable ? 8 : 1;

        for (uint32_t i = 0; i < rtCount; ++i)
        {
            if (mDesc.renderTarget[i].blendEnable)
            {
                VGL( glEnablei, GL_BLEND, i );

                uint32_t srcBlend      = BlendState_GetBlendFactor(mDesc.renderTarget[i].srcBlend);
                uint32_t dstBlend      = BlendState_GetBlendFactor(mDesc.renderTarget[i].dstBlend);
                uint32_t blendOp       = BlendState_GetBlendOp    (mDesc.renderTarget[i].blendOp);

                uint32_t srcBlendAlpha = BlendState_GetBlendFactor(mDesc.renderTarget[i].srcBlendAlpha);
                uint32_t dstBlendAlpha = BlendState_GetBlendFactor(mDesc.renderTarget[i].dstBlendAlpha);
                uint32_t blendOpAlpha  = BlendState_GetBlendOp    (mDesc.renderTarget[i].blendOpAlpha);

                VGL( glBlendFuncSeparatei, i, srcBlend, dstBlend, srcBlendAlpha, dstBlendAlpha );
                VGL( glBlendEquationSeparatei, i, blendOp, blendOpAlpha );
            }
            else
            {
                VGL( glDisablei, GL_BLEND, i );
            }

            // This state setup is out of condition scope as
            // it seems that it isn't affected by enabling or disabling blend state
            uint32_t mask = mDesc.renderTarget[i].writeMask;
            uint8_t maskR = (mask & COLOR_MASK_RED)   ? GL_TRUE : GL_FALSE;
            uint8_t maskG = (mask & COLOR_MASK_GREEN) ? GL_TRUE : GL_FALSE;
            uint8_t maskB = (mask & COLOR_MASK_BLUE)  ? GL_TRUE : GL_FALSE;
            uint8_t maskA = (mask & COLOR_MASK_ALPHA) ? GL_TRUE : GL_FALSE;

            VGL( glColorMaski, i, maskR, maskG, maskB, maskA );
        }

        VGL( glBlendColor, mDesc.blendFactor[0], mDesc.blendFactor[1], mDesc.blendFactor[2], mDesc.blendFactor[3] );
    }

} // namespace GFW
