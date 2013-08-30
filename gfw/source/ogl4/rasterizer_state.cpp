#include "common/trace.h"

#include "gfw/common/device_child.inl"

#include "gfw/core/rasterizer_state.h"
#include "gfw/core/functions.h"

namespace GFW {

    RasterizerState::RasterizerState(const RasterizerStateDesc & desc, uint32_t descHash, IDeviceRef device)
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
            TRACE_ASSERT_GL(glPolygonMode, GL_FRONT_AND_BACK, GL_LINE);
            break;
        case FILLMODE_SOLID:
            TRACE_ASSERT_GL(glPolygonMode, GL_FRONT_AND_BACK, GL_FILL);
            break;
        default:
            TRACE_FAIL();
            TRACE_ASSERT_GL(glPolygonMode, GL_FRONT_AND_BACK, GL_FILL);
            break;
        }

        // setup front face orientation
        TRACE_ASSERT_GL(glFrontFace, mDesc.frontCounterClockwise ? GL_CCW : GL_CW);

        // setup primitives culling mode
        switch (mDesc.cullMode)
        {
        case CULLMODE_BACK:
            TRACE_ASSERT_GL(glEnable, GL_CULL_FACE);
            TRACE_ASSERT_GL(glCullFace, GL_BACK);
            break;
        case CULLMODE_FRONT:
            TRACE_ASSERT_GL(glEnable, GL_CULL_FACE);
            TRACE_ASSERT_GL(glCullFace, GL_FRONT);
            break;
        case CULLMODE_NONE:
            TRACE_ASSERT_GL(glDisable, GL_CULL_FACE);
            break;
        default:
            TRACE_FAIL();
            TRACE_ASSERT_GL(glDisable, GL_CULL_FACE);
            break;
        }

        // enable depth bias for those types of primitives which are enabled by fill mode
        switch (mDesc.fillMode)
        {
        case FILLMODE_WIREFRAME:
            TRACE_ASSERT_GL(glEnable, GL_POLYGON_OFFSET_LINE);
            TRACE_ASSERT_GL(glDisable, GL_POLYGON_OFFSET_FILL);
            break;
        case FILLMODE_SOLID:
            TRACE_ASSERT_GL(glEnable, GL_POLYGON_OFFSET_FILL);
            TRACE_ASSERT_GL(glDisable, GL_POLYGON_OFFSET_LINE);
            break;
        default:
            TRACE_FAIL();
            TRACE_ASSERT_GL(glEnable, GL_POLYGON_OFFSET_FILL);
            TRACE_ASSERT_GL(glDisable, GL_POLYGON_OFFSET_LINE);
            break;
        }
        // setup depth bias parameters
        // DepthBiasClamp is always [0, 1] for OpenGL
        TRACE_ASSERT_GL(glPolygonOffset, mDesc.slopeScaleDepthBias, mDesc.depthBias);

        // Setup scissor test
        if (mDesc.scissorEnable)
        {
            TRACE_ASSERT_GL(glEnable, GL_SCISSOR_TEST);
        }
        else
        {
            TRACE_ASSERT_GL(glDisable, GL_SCISSOR_TEST);
        }

        if (mDesc.multisampleEnable)
        {
            TRACE_ASSERT_GL(glEnable, GL_MULTISAMPLE);
        }
        else
        {
            TRACE_ASSERT_GL(glDisable, GL_MULTISAMPLE);
        }

        if (mDesc.antialiasedLineEnable)
        {
            TRACE_ASSERT_GL(glEnable, GL_LINE_SMOOTH);
        }
        else
        {
            TRACE_ASSERT_GL(glDisable, GL_LINE_SMOOTH);
        }
    }

} // namespace GFW
