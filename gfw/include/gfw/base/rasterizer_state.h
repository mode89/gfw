#ifndef __GFW_BASE_RASTERIZER_STATE_H__
#define __GFW_BASE_RASTERIZER_STATE_H__

#include "common/autoref.h"

namespace GFW {

    /// Determines the fill mode to use when rendering triangles.
    enum FillMode
    {
        FILLMODE_WIREFRAME,     ///< Draw lines connecting the vertices. Adjacent vertices are not drawn.
        FILLMODE_SOLID,         ///< Fill the triangles formed by the vertices. Adjacent vertices are not drawn.
        FILLMODE_FORCESIZE = 0x7FFFFFFF
    };

    /// Indicates triangles facing a particular direction are not drawn.
    enum CullMode
    {
        CULLMODE_NONE,      ///< Always draw all triangles.
        CULLMODE_FRONT,     ///< Do not draw triangles that are front-facing.
        CULLMODE_BACK,      ///< Do not draw triangles that are back-facing.
        CULLMODE_FORCESIZE = 0x7FFFFFFF
    };

    /// Describes rasterizer state.
    struct RasterizerStateDesc
    {
        FillMode                fillMode;               ///< Determines the fill mode to use when rendering.
        CullMode                cullMode;               ///< Indicates triangles facing the specified direction are not drawn.
        float                   depthBias;              ///< Depth value added to a given pixel.
        float                   depthBiasClamp;         ///< Maximum depth bias of a pixel.
        float                   slopeScaleDepthBias;    ///< Scalar on a given pixel's slope.
        uint8_t                 depthClipEnable;        ///< Enable clipping based on distance.
        uint8_t                 scissorEnable;          ///< Enable scissor-rectangle culling. All pixels ouside an active scissor rectangle are culled.
        uint8_t                 multisampleEnable;      ///< Enable multisample antialiasing. Must be combined with a multisampled rendertarget.
        uint8_t                 antialiasedLineEnable;  ///< Enable line antialiasing; only applies if doing line drawing and multisampleEnable is false.
        uint8_t                 frontCounterClockwise;  ///< Determines if a triangle is front- or back-facing. If this parameter is true, then a triangle will be considered front-facing if its vertices are counter-clockwise on the render target and considered back-facing if they are clockwise. If this parameter is false then the opposite is true.

        RasterizerStateDesc()
            : fillMode(FILLMODE_SOLID)
            , cullMode(CULLMODE_BACK)
            , depthBias(0.f)
            , depthBiasClamp(0.f)
            , slopeScaleDepthBias(0.f)
            , depthClipEnable(1)
            , scissorEnable(0)
            , multisampleEnable(0)
            , antialiasedLineEnable(0)
            , frontCounterClockwise(0)
        {}
    };

    class IRasterizerState : public Common::ARefCounted
    {
    public:
        virtual void
        Apply() const = 0;

        ~IRasterizerState() {}
    };

} // namespace GFW

#endif // __GFW_BASE_RASTERIZER_STATE_H__
