#ifndef __GFW_BASE_SHADER_REFLECT_H__
#define __GFW_BASE_SHADER_REFLECT_H__

#include "gfw/base/device_child.h"
#include "gfw/base/types_fwd.h"
#include "gfw/base/semantic.h"

namespace GFW {

    // ***********************************************************************

    struct ShaderDesc
    {
        uint32_t    variableCount;
        uint32_t    cbufferCount;
        uint32_t    resourceCount;
        uint32_t    inputsCount;

        ShaderDesc()
            : variableCount(0)
            , cbufferCount(0)
            , resourceCount(0)
            , inputsCount(0)
        {}
    };

    class IShaderReflection : public IDeviceChild
    {
    public:
        virtual ShaderDesc &
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
        GetInputParameter(const char * name) = 0;

        virtual
        ~IShaderReflection() {}
    };
    AUTOREF_REFERENCE_DECLARATION(IShaderReflection);

    // ***********************************************************************

    enum ShaderVariableType
    {
        SHADER_VAR_TYPE_UNKNOWN = 0,
    };

    struct ShaderVariableDesc
    {
        ShaderVariableType  type;
        uint32_t            location;
        uint32_t            size;
        int32_t             bufferIndex;

        ShaderVariableDesc()
            : type(SHADER_VAR_TYPE_UNKNOWN)
            , location(0)
            , size(0)
            , bufferIndex(-1)
        {}
    };

    class IShaderVariable : public Common::ARefCounted
    {
    public:
        virtual ShaderVariableDesc &
        GetDesc() = 0;

        virtual const char *
        GetName() = 0;

        virtual
        ~IShaderVariable() {}
    };
    AUTOREF_REFERENCE_DECLARATION(IShaderVariable);

    // ***********************************************************************

    struct ShaderBufferDesc
    {
        uint32_t    size;

        ShaderBufferDesc()
            : size(0)
        {}
    };

    class IShaderBuffer : public Common::ARefCounted
    {
    public:
        virtual ShaderBufferDesc &
        GetDesc() = 0;

        virtual const char *
        GetName() = 0;

        virtual
        ~IShaderBuffer() {}
    };
    AUTOREF_REFERENCE_DECLARATION(IShaderBuffer);

    // ***********************************************************************

    enum ShaderResourceType
    {
        SHADER_RES_TYPE_UNKNOWN = 0,
    };

    enum ShaderResourceDim
    {
        SHADER_RES_DIM_UNKNOWN = 0,
    };

    struct ShaderResourceDesc
    {
        ShaderResourceType  type;
        ShaderResourceDim   dim;
        uint32_t            bindPoint;
        uint32_t            bindCount;

        ShaderResourceDesc()
            : type(SHADER_RES_TYPE_UNKNOWN)
            , dim(SHADER_RES_DIM_UNKNOWN)
            , bindPoint(0)
            , bindCount(0)
        {}
    };

    class IShaderResource : public Common::ARefCounted
    {
    public:
        virtual ShaderResourceDesc &
        GetDesc() = 0;

        virtual const char *
        GetName() = 0;

        virtual
        ~IShaderResource() {}
    };
    AUTOREF_REFERENCE_DECLARATION(IShaderResource);

    // ***********************************************************************

    struct ShaderParameterDesc
    {
        Semantic    semantic;
        uint32_t    location;

        ShaderParameterDesc()
            : semantic(SEMANTIC_UNKNOWN)
            , location(0)
        {}
    };

    class IShaderParameter : public Common::ARefCounted
    {
    public:
        virtual ShaderParameterDesc &
        GetDesc() = 0;

        virtual const char *
        GetName() = 0;

        virtual
        ~IShaderParameter() {}
    };
    AUTOREF_REFERENCE_DECLARATION(IShaderParameter);

} // namespace GFW

#endif // __GFW_BASE_SHADER_REFLECT_H__
