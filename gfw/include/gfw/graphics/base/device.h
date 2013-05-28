#ifndef __GFW_GRAPHICS_BASE_DEVICE_H__
#define __GFW_GRAPHICS_BASE_DEVICE_H__

#include "common\autoref.h"
#include "gfw\platform\base\window.h"
#include "gfw\graphics\base\context.h"
#include "gfw\graphics\base\shader.h"
#include "gfw\graphics\base\buffer.h"

namespace GFW {

    enum DeviceType
    {
        DEVICE_NULL = 0,
        DEVICE_OPENGL
    };

    class IDevice: public Common::ARefCounted
    {
    public:

        virtual IContextRef     CreateContext(GFW::Platform::IWindowIn) = 0;

        virtual IShaderRef      CreateShader(ShaderStage, const void * shaderData) = 0;

        virtual IBufferRef      CreateBuffer(const BufferDesc &, const void * initialData) = 0;

    public:
        virtual                 ~IDevice() { }
    };
	AUTOREF_REFERENCE_DECLARATION(IDevice);

} // namespace GFW

#endif // __GFW_GRAPHICS_BASE_DEVICE_H__
