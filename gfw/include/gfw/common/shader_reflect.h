#ifndef __GFW_COMMON_SHADER_REFLECT_H__
#define __GFW_COMMON_SHADER_REFLECT_H__

#include "gfw/base/shader_reflect.h"
#include "gfw/base/types_fwd.h"
#include "gfw/common/device_child.h"

#include <unordered_map>
#include <vector>

namespace GFW {

    class ShaderVariable : public IShaderVariable
    {
    public:
        virtual const char *
        GetName() { return mName; }

        virtual const ShaderVariableDesc &
        GetDesc() { return mDesc; }

    public:
        ShaderVariable();
        ~ShaderVariable();

    private:
        char *              mName;
        ShaderVariableDesc  mDesc;
    };
    AUTOREF_REFERENCE_DECLARATION(ShaderVariable);

    class ShaderBuffer : public IShaderBuffer
    {
    public:
        virtual const char *
        GetName() { return mName; }

        virtual const ShaderBufferDesc &
        GetDesc() { return mDesc; }

    public:
        ShaderBuffer();
        ~ShaderBuffer();

    private:
        char *              mName;
        ShaderBufferDesc    mDesc;
    };
    AUTOREF_REFERENCE_DECLARATION(ShaderBuffer);

    class ShaderResource : public IShaderResource
    {
    public:
        virtual const char *
        GetName() { return mName; }

        virtual const ShaderResourceDesc &
        GetDesc() { return mDesc; }

    public:
        ShaderResource();
        ~ShaderResource();

    private:
        char *              mName;
        ShaderResourceDesc  mDesc;
    };
    AUTOREF_REFERENCE_DECLARATION(ShaderResource);

    class ShaderParameter : public IShaderParameter
    {
    public:
        virtual const char *
        GetName() { return mName; }

        virtual const ShaderParameterDesc &
        GetDesc() { return mDesc; }

    public:
        ShaderParameter();
        ~ShaderParameter();

    private:
        char *              mName;
        ShaderParameterDesc mDesc;
    };
    AUTOREF_REFERENCE_DECLARATION(ShaderParameter);

    template < class Base >
    class AShaderReflection : public ADeviceChild<Base>
    {
    public:
        virtual const ShaderDesc &
        GetDesc() { return mDesc; }

        virtual IShaderVariableRef
        GetVariable(uint32_t index);

        virtual IShaderVariableRef
        GetVariable(const char * name);

        virtual IShaderBufferRef
        GetBuffer(uint32_t index);

        virtual IShaderBufferRef
        GetBuffer(const char * name);

        virtual IShaderResourceRef
        GetResource(uint32_t index);

        virtual IShaderResourceRef
        GetResource(const char * name);

        virtual IShaderParameterRef
        GetInputParameter(uint32_t index);

        virtual IShaderParameterRef
        GetInputParameter(const char * name);

    public:
        void Initialize(const ShaderDesc &);

    public:
        AShaderReflection(IDeviceIn);
        ~AShaderReflection();

    private:
        typedef std::vector < ShaderVariableRef >   ShaderVariableVec;
        typedef std::vector < ShaderBufferRef >     ShaderBufferVec;
        typedef std::vector < ShaderResourceRef >   ShaderResourceVec;
        typedef std::vector < ShaderParameterRef >  ShaderParameterVec;

        typedef std::unordered_map < const char *, ShaderVariableRef >   ShaderVariableMap;
        typedef std::unordered_map < const char *, ShaderBufferRef >     ShaderBufferMap;
        typedef std::unordered_map < const char *, ShaderResourceRef >   ShaderResourceMap;
        typedef std::unordered_map < const char *, ShaderParameterRef >  ShaderParameterMap;

        ShaderVariableVec   mVariables;
        ShaderBufferVec     mBuffers;
        ShaderResourceVec   mResources;
        ShaderParameterVec  mInputs;

        ShaderVariableMap   mVariablesMap;
        ShaderBufferMap     mBuffersMap;
        ShaderResourceMap   mResourcesMap;
        ShaderParameterMap  mInputsMap;

        ShaderDesc          mDesc;
    };

} // namespace GFW

#endif // __GFW_COMMON_SHADER_REFLECT_H__
