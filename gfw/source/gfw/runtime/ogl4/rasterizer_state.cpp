#include "cmn/trace.h"
#include "gfw/runtime/common/device_child.inl"
#include "gfw/runtime/ogl4/device.h"
#include "gfw/runtime/ogl4/functions.h"
#include "gfw/runtime/ogl4/rasterizer_state.h"

namespace GFW {

    RasterizerState::RasterizerState( const RasterizerStateDesc & desc, uint32_t descHash, DeviceIn device )
        : ADeviceChild(device)
        , mDesc(desc)
    { }

    RasterizerState::~RasterizerState()
    { }

    void RasterizerState::Apply() const
    {
        // setup rasterization method for polygons
        switch (mDesc.fillMode)
        {
        case FILLMODE_WIREFRAME:
            VGL( glPolygonMode, GL_FRONT_AND_BACK, GL_LINE );
            break;
        case FILLMODE_SOLID:
            VGL( glPolygonMode, GL_FRONT_AND_BACK, GL_FILL );
            break;
        default:
            CMN_FAIL();
            VGL( glPolygonMode, GL_FRONT_AND_BACK, GL_FILL );
            break;
        }

        // setup front face orientation
        VGL( glFrontFace, mDesc.frontCounterClockwise ? GL_CCW : GL_CW );

        // setup primitives culling mode
        switch (mDesc.cullMode)
        {
        case CULLMODE_BACK:
            VGL( glEnable, GL_CULL_FACE );
            VGL( glCullFace, GL_BACK );
            break;
        case CULLMODE_FRONT:
            VGL( glEnable, GL_CULL_FACE );
            VGL( glCullFace, GL_FRONT );
            break;
        case CULLMODE_NONE:
            VGL( glDisable, GL_CULL_FACE );
            break;
        default:
            CMN_FAIL();
            VGL( glDisable, GL_CULL_FACE );
            break;
        }

        // enable depth bias for those types of primitives which are enabled by fill mode
        switch (mDesc.fillMode)
        {
        case FILLMODE_WIREFRAME:
            VGL( glEnable, GL_POLYGON_OFFSET_LINE );
            VGL( glDisable, GL_POLYGON_OFFSET_FILL );
            break;
        case FILLMODE_SOLID:
            VGL( glEnable, GL_POLYGON_OFFSET_FILL );
            VGL( glDisable, GL_POLYGON_OFFSET_LINE );
            break;
        default:
            CMN_FAIL();
            VGL( glEnable, GL_POLYGON_OFFSET_FILL );
            VGL( glDisable, GL_POLYGON_OFFSET_LINE );
            break;
        }
        // setup depth bias parameters
        // DepthBiasClamp is always [0, 1] for OpenGL
        VGL( glPolygonOffset, mDesc.slopeScaleDepthBias, mDesc.depthBias );

        // Setup scissor test
        if (mDesc.scissorEnable)
        {
            VGL( glEnable, GL_SCISSOR_TEST );
        }
        else
        {
            VGL( glDisable, GL_SCISSOR_TEST );
        }

        if (mDesc.multisampleEnable)
        {
            VGL( glEnable, GL_MULTISAMPLE );
        }
        else
        {
            VGL( glDisable, GL_MULTISAMPLE );
        }

        if (mDesc.antialiasedLineEnable)
        {
            VGL( glEnable, GL_LINE_SMOOTH );
        }
        else
        {
            VGL( glDisable, GL_LINE_SMOOTH );
        }
    }

} // namespace GFW
