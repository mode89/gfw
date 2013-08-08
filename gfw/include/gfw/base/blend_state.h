#ifndef __GFW_BASE_BLEND_STATE_H__
#define __GFW_BASE_BLEND_STATE_H__

#include "common/autoref.h"

#include "gfw/base/compare.h"

namespace GFW {

    enum ColorMask
    {
        COLOR_MASK_RED   = (1u << 0),
        COLOR_MASK_GREEN = (1u << 1),
        COLOR_MASK_BLUE  = (1u << 2),
        COLOR_MASK_ALPHA = (1u << 3),
        COLOR_MASK_ALL   = (COLOR_MASK_RED | COLOR_MASK_GREEN | COLOR_MASK_BLUE | COLOR_MASK_ALPHA)
    };

    /// Blend factor. A blend factor identifies the data source and an optional pre-blend operation.
    enum BlendFactor
    {
        BLEND_FACTOR_ZERO,               ///< The data source is the color black (0, 0, 0, 0). No pre-blend operation.
        BLEND_FACTOR_ONE,                ///< The data source is the color white (1, 1, 1, 1). No pre-blend operation.
        BLEND_FACTOR_SRC_COLOR,          ///< The data source is color data (RGB) from a pixel shader. No pre-blend operation.
        BLEND_FACTOR_INV_SRC_COLOR,      ///< The data source is color data (RGB) from a pixel shader. The pre-blend operation inverts the data, generating 1-RGB.
        BLEND_FACTOR_SRC_ALPHA,          ///< The data source is alpha data (RGB) from a pixel shader. No pre-blend operation.
        BLEND_FACTOR_INV_SRC_ALPHA,      ///< The data source is alpha data (RGB) from a pixel shader. The pre-blend operation inverts the data, generating 1-A.
        BLEND_FACTOR_DEST_ALPHA,         ///< The data source is alpha data from a rendertarget. No pre-blend operation.
        BLEND_FACTOR_INV_DEST_ALPHA,     ///< The data source is alpha data from a rendertarget. The pre-blend operation inverts the data, generating 1-A.
        BLEND_FACTOR_DEST_COLOR,         ///< The data source is color data (RGB) from a rendertarget. No pre-blend operation.
        BLEND_FACTOR_INV_DEST_COLOR,     ///< The data source is color data (RGB) from a rendertarget. The pre-blend operation inverts the data, generating 1-RGB.
        BLEND_FACTOR_SRC_ALPHA_SAT,      ///< The data source is alpha data from a pixel shader. The pre-blend operation clamps the data to 1 or less.
        BLEND_FACTOR_CONSTANT,           ///< The data source is the blend factor. No pre-blend operation.
        BLEND_FACTOR_INV_CONSTANT,       ///< The data source is the blend factor. The pre-blend operation inverts the blend factor, generating 1-blend_factor.
        BLEND_FACTOR_DUAL_SRC_COLOR,     ///< The data sources are both color data output by a pixel shader. There is no pre-blend operation. This options supports dual-source color blending.
        BLEND_FACTOR_INV_DUAL_SRC_COLOR, ///< The data sources are both color data output by a pixel shader. The pre-blend operation inverts the data, generating 1-RGB. This options supports dual-source color blending.
        BLEND_FACTOR_DUAL_SRC_ALPHA,     ///< The data sources are alpha data output by a pixel shader. There is no pre-blend operation. This options supports dual-source color blending.
        BLEND_FACTOR_INV_DUAL_SRC_ALPHA, ///< The data sources are alpha data output by a pixel shader. The pre-blend operation inverts the data, generating 1-A. This options supports dual-source color blending.
    };

    /// RGB or alpha blending operation.
    enum BlendOp
    {
        BLEND_OP_ADD,                ///< Add source 1 and source 2.
        BLEND_OP_SUBTRACT,           ///< Subtract source 1 from source 2.
        BLEND_OP_REVERSE_SUBTRACT,   ///< Subtract source 2 from source 1.
        BLEND_OP_MIN,                ///< Find the minimum of source 1 and source 2.
        BLEND_OP_MAX,                ///< Find the maximum of source 1 and source 2.
    };

    /// Describes the blend state for a render target.
    struct RenderTargetBlendDesc
    {
        BlendFactor             srcBlend;       ///< Specifies the first RGB data source and includes an optional pre-blend operation.
        BlendFactor             dstBlend;       ///< Specifies the second RGB data source and includes an optional pre-blend operation.
        BlendOp                 blendOp;        ///< Defines how to combine the RGB data sources.
        BlendFactor             srcBlendAlpha;  ///< Specifies the first alpha data source and includes an optional pre-blend operation. Blend-factors that end in _COLOR are not allowed.
        BlendFactor             dstBlendAlpha;  ///< Specifies the second alpha data source and includes an optional pre-blend operation. Blend-factors that end in _COLOR are not allowed.
        BlendOp                 blendOpAlpha;   ///< Defines how to combine the alpha data sources.
        uint32_t                writeMask;      ///< Mask specifying which channels of the rendertarget will be written to. OR'd combination of ColorMask.
        bool8_t                 blendEnable;    ///< Enable (or disable) blending.

        RenderTargetBlendDesc()
            : srcBlend(BLEND_FACTOR_ONE)
            , dstBlend(BLEND_FACTOR_ZERO)
            , blendOp(BLEND_OP_ADD)
            , srcBlendAlpha(BLEND_FACTOR_ONE)
            , dstBlendAlpha(BLEND_FACTOR_ZERO)
            , blendOpAlpha(BLEND_OP_ADD)
            , writeMask(COLOR_MASK_ALL) 
            , blendEnable(0)
        {}
    };

    /// Describes the blend state.
    struct BlendStateDesc
    {
        float                   blendFactor[4];         ///< Blend factor used when specifying BLEND_FACTOR_CONSTANT/BLEND_FACTOR_INV_CONSTANT
        RenderTargetBlendDesc   renderTarget[8];        ///< An array of render-target-blend descriptions that correspond to the rendertargets that can be bound to the output-merger stage.
        CompareFunc             alphaFunc;              ///< Fixed-function alpha test.
        uint32_t                alphaRef;               ///< Fixed-function alpha test.
        uint32_t                sampleMask;             ///< Coverage mask that controls which samples are written to a surface.
        bool8_t                 alphaToCoverageEnable;  ///< Determines whether or not to use alpha-to-coverage as a multisampling technique when setting a pixel to a rendertarget.
        bool8_t                 independentBlendEnable; ///< Set to true to enable independent blending in simultaneous render targets. If set to false, only the renderTarget[0] members are used and renderTarget[1..7] are ignored.
        bool8_t                 alphaTestEnable;        ///< Enable (or disable) fixed-function alpha testing.

        BlendStateDesc()
            : alphaFunc(COMPARE_ALWAYS)
            , alphaRef(0)
            , sampleMask(0xFFFFFFFF)
            , alphaToCoverageEnable(0)
            , independentBlendEnable(0)
            , alphaTestEnable(0)
        {
            blendFactor[0] = 0.0f;
            blendFactor[1] = 0.0f;
            blendFactor[2] = 0.0f;
            blendFactor[3] = 0.0f;
        }
    };

    class IBlendState : public Common::ARefCounted
    {
    public:
        virtual void    Apply() const = 0;
        virtual         ~IBlendState() {}
    };

} // namespace GFW

#endif // __GFW_BASE_BLEND_STATE_H__
