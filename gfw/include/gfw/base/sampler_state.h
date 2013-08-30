#ifndef __GFW_BASE_SAMPLER_STATE_H__
#define __GFW_BASE_SAMPLER_STATE_H__

#include "gfw/base/compare.h"
#include "gfw/base/types_fwd.h"

#include <float.h>

namespace GFW {

    /// Filtering options during texture sampling.
    enum Filter
    {
        FILTER_UNKNOWN = -1,
        FILTER_MIN_MAG_MIP_POINT,
        FILTER_MIN_MAG_POINT_MIP_LINEAR,
        FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT,
        FILTER_MIN_POINT_MAG_MIP_LINEAR,
        FILTER_MIN_LINEAR_MAG_MIP_POINT,
        FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
        FILTER_MIN_MAG_LINEAR_MIP_POINT,
        FILTER_MIN_MAG_MIP_LINEAR,
        FILTER_ANISOTROPIC,
        FILTER_COMPARE_MIN_MAG_MIP_POINT,
        FILTER_COMPARE_MIN_MAG_POINT_MIP_LINEAR,
        FILTER_COMPARE_MIN_POINT_MAG_LINEAR_MIP_POINT,
        FILTER_COMPARE_MIN_POINT_MAG_MIP_LINEAR,
        FILTER_COMPARE_MIN_LINEAR_MAG_MIP_POINT,
        FILTER_COMPARE_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
        FILTER_COMPARE_MIN_MAG_LINEAR_MIP_POINT,
        FILTER_COMPARE_MIN_MAG_MIP_LINEAR,
        FILTER_COMPARE_ANISOTROPIC,
        FILTER_TEXT_1BIT,
    };

    /// Identify a technique for resolving texture coordinates that are outside of the boundaries of a texture.
    enum AddressMode
    {
        ADDRESSMODE_WRAP,           ///< Tile the texture at every (u,v,w) integer junction.
        ADDRESSMODE_MIRROR,         ///< Flip the texture at every (u,v,w) integer junction.
        ADDRESSMODE_CLAMP,          ///< Texture coordinates outside the range [0.0, 1.0] are set to the texture color at 0.0 or 1.0, respectively.
        ADDRESSMODE_BORDER,         ///< Texture coordinates outside the range [0.0, 1.0] are set to the border color specified in SamplerStateDescriptor.
        ADDRESSMODE_MIRROR_ONCE,    ///< Similar to ADDRESSMODE_MIRROR and ADDRESSMODE_CLAMP. Takes the absolute value of the texture coordinate (thus, mirroring around 0), and then clamps to the maximum value.
    };

    /// Describes a sampler state.
    struct SamplerStateDesc
    {
        float           borderColor[4]; ///< Border color to use if ADDRESSMODE_BORDER is specified for addressU, addressV, or addressW (range must be between 0.0 and 1.0 inclusive).
        int32_t         maxAnisotropy;  ///< Clamping value used if FILTER_ANISOTROPIC is specified in filter. Valid values are between 1 and 16.
        float           mipLodBias;     ///< Offset from the calculated mipmap level.
        float           minLod;         ///< Lower end of the mipmap range to clamp access to, where 0 is the largest and most detailed mipmap level and any level higher than that is less detailed.
        float           maxLod;         ///< Upper end of the mipmap range to clamp access to, where 0 is the largest and most detailed mipmap level and any level higher than that is less detailed. This value must be greater than or equal to minLod. To have no upper limit on LOD set this to a large value such as FLOAT_MAX.
        Filter          filter;
        AddressMode     addressU;       ///< Method to use for resolving a U texture coordinate that is outside the 0 to 1 range.
        AddressMode     addressV;       ///< Method to use for resolving a V texture coordinate that is outside the 0 to 1 range.
        AddressMode     addressW;       ///< Method to use for resolving a W texture coordinate that is outside the 0 to 1 range.
        CompareFunc     comparison;     ///< A function that compares sampled data against existing sampled data.

        SamplerStateDesc()
            : maxAnisotropy(16)
            , mipLodBias(0.f)
            , minLod(0.f)
            , maxLod(FLT_MAX)
            , filter(FILTER_MIN_MAG_MIP_LINEAR)
            , addressU(ADDRESSMODE_CLAMP)
            , addressV(ADDRESSMODE_CLAMP)
            , addressW(ADDRESSMODE_CLAMP)
            , comparison(COMPARE_NEVER)
        {
            for (int i = 0; i < 4; ++ i)
            {
                borderColor[i] = 0.0f;
            }
        }
    };

    class ISamplerState : public Common::ARefCounted
    {
    public:
        virtual void
        Apply() = 0;

        ~ISamplerState() {}
    };
    AUTOREF_REFERENCE_DECLARATION(ISamplerState);

} // namespace GFW

#endif // __GFW_BASE_SAMPLER_STATE_H__
