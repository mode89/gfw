#ifndef __GFW_BASE_SHADER_REFLECT_H__
#define __GFW_BASE_SHADER_REFLECT_H__

#include "gfw/base/device_child.h"
#include "gfw/base/types_fwd.h"
#include "gfw/shared/semantic.h"

namespace GFW {

    class IShaderReflection : public IDeviceChild
    {
    public:
        virtual const ShaderDesc &
        GetDesc() = 0;

        virtual IShaderVariableRef
        GetVariable(uint32_t index) = 0;

        virtual IShaderVariableRef
        GetVariable(const char * name) = 0;

        virtual IShaderBufferRef
        GetBuffer(uint32_t index) = 0;

        virtual IShaderBufferRef
        GetBuffer(const char * name) = 0;

        virtual IShaderResourceRef
        GetResource(uint32_t index) = 0;

        virtual IShaderResourceRef
        GetResource(const char * name) = 0;

        virtual IShaderParameterRef
        GetInputParameter(uint32_t index) = 0;

        virtual IShaderParameterRef
        GetInputParameter(Semantic) = 0;

        virtual
        ~IShaderReflection() {}
    };
    AUTOREF_REFERENCE_DECLARATION(IShaderReflection);

    class IShaderVariable : public Common::ARefCounted
    {
    public:
        virtual const ShaderVariableDesc &
        GetDesc() = 0;

        virtual const char *
        GetName() = 0;

        virtual
        ~IShaderVariable() {}
    };
    AUTOREF_REFERENCE_DECLARATION(IShaderVariable);

    class IShaderBuffer : public Common::ARefCounted
    {
    public:
        virtual const ShaderBufferDesc &
        GetDesc() = 0;

        virtual const char *
        GetName() = 0;

        virtual
        ~IShaderBuffer() {}
    };
    AUTOREF_REFERENCE_DECLARATION(IShaderBuffer);

    class IShaderResource : public Common::ARefCounted
    {
    public:
        virtual const ShaderResourceDesc &
        GetDesc() = 0;

        virtual const char *
        GetName() = 0;

        virtual
        ~IShaderResource() {}
    };
    AUTOREF_REFERENCE_DECLARATION(IShaderResource);

    class IShaderParameter : public Common::ARefCounted
    {
    public:
        virtual const ShaderParameterDesc &
        GetDesc() = 0;

        virtual
        ~IShaderParameter() {}
    };
    AUTOREF_REFERENCE_DECLARATION(IShaderParameter);

} // namespace GFW

#endif // __GFW_BASE_SHADER_REFLECT_H__
