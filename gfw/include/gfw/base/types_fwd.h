#ifndef __GFW_BASE_TYPES_FWD_H__
#define __GFW_BASE_TYPES_FWD_H__

#include "common/autoref.h"

namespace GFW {

    AUTOREF_FORWARD_DECLARATION(IBuffer);
    AUTOREF_FORWARD_DECLARATION(IContext);
    AUTOREF_FORWARD_DECLARATION(IDevice);
    AUTOREF_FORWARD_DECLARATION(IDeviceChild);
    AUTOREF_FORWARD_DECLARATION(IRenderBuffer);
    AUTOREF_FORWARD_DECLARATION(IResource);
    AUTOREF_FORWARD_DECLARATION(IShader);
    AUTOREF_FORWARD_DECLARATION(ITexture);

    struct BufferDesc;
    struct ClearParams;
    struct DrawParams;
    struct DrawIndexedParams;
    struct ResourceDesc;
    struct TextureDesc;
    struct SubResIdx;
    struct VertexAttribute;

} // namespace GFW

#endif // __GFW_BASE_TYPES_FWD_H__
