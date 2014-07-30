#ifndef __GFW_BASE_SHADER_REFLECT_H__
#define __GFW_BASE_SHADER_REFLECT_H__

#include "gfw/device_child.h"
#include "gfw/types_fwd.h"
#include "gfw/shared/semantic.h"

namespace GFW {

    class IShaderReflection : public IDeviceChild
    {
    public:
        virtual const ShaderDesc &
        GetDesc() const = 0;

        virtual ConstIShaderVariableRef
        GetVariable(uint32_t index) const = 0;

        virtual ConstIShaderVariableRef
        GetVariable(const char * name) const = 0;

        virtual ConstIShaderBufferRef
        GetBuffer(uint32_t index) const = 0;

        virtual ConstIShaderBufferRef
        GetBuffer(const char * name) const = 0;

        virtual ConstIShaderResourceRef
        GetResource(uint32_t index) const = 0;

        virtual ConstIShaderResourceRef
        GetResource(const char * name) const = 0;

        virtual ConstIShaderParameterRef
        GetInputParameter(uint32_t index) const = 0;

        virtual ConstIShaderParameterRef
        GetInputParameter(Semantic) const = 0;

        virtual
        ~IShaderReflection() {}
    };
    SHARED_PTR_TYPEDEFS(IShaderReflection);

    class IShaderVariable
    {
    public:
        virtual const ShaderVariableDesc &
        GetDesc() const = 0;

        virtual const char *
        GetName() const = 0;

        virtual
        ~IShaderVariable() {}
    };
    SHARED_PTR_TYPEDEFS(IShaderVariable);

    class IShaderBuffer
    {
    public:
        virtual const ShaderBufferDesc &
        GetDesc() const = 0;

        virtual const char *
        GetName() const = 0;

        virtual
        ~IShaderBuffer() {}
    };
    SHARED_PTR_TYPEDEFS(IShaderBuffer);

    class IShaderResource
    {
    public:
        virtual const ShaderResourceDesc &
        GetDesc() const = 0;

        virtual const char *
        GetName() const = 0;

        virtual
        ~IShaderResource() {}
    };
    SHARED_PTR_TYPEDEFS(IShaderResource);

    class IShaderParameter
    {
    public:
        virtual const ShaderParameterDesc &
        GetDesc() const = 0;

        virtual
        ~IShaderParameter() {}
    };
    SHARED_PTR_TYPEDEFS(IShaderParameter);

} // namespace GFW

#endif // __GFW_BASE_SHADER_REFLECT_H__
