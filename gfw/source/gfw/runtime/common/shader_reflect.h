#ifndef __GFW_RUNTIME_COMMON_SHADER_REFLECT_H__
#define __GFW_RUNTIME_COMMON_SHADER_REFLECT_H__

#include "gfw/shader_reflect.h"
#include "gfw/types_fwd.h"
#include "gfw/runtime/common/device_child.h"
#include "gfw/shared/shader.h"

#include <unordered_map>
#include <vector>

namespace GFW {

    class ShaderVariable : public IShaderVariable
    {
    public:
        virtual const char *
        GetName() const { return mName; }

        virtual const ShaderVariableDesc &
        GetDesc() const { return mDesc; }

    public:
        ShaderVariable( const char * name, const ShaderVariableDesc & desc ) { mName = name; mDesc = desc; }

    private:
        const char *        mName;
        ShaderVariableDesc  mDesc;
    };
    SHARED_PTR_TYPEDEFS( ShaderVariable );

    class ShaderBuffer : public IShaderBuffer
    {
    public:
        virtual const char *
        GetName() const { return mName; }

        virtual const ShaderBufferDesc &
        GetDesc() const { return mDesc; }

    public:
        ShaderBuffer( const char * name, const ShaderBufferDesc & desc ) { mName = name; mDesc = desc; }

    private:
        const char *        mName;
        ShaderBufferDesc    mDesc;
    };
    SHARED_PTR_TYPEDEFS( ShaderBuffer );

    class ShaderResource : public IShaderResource
    {
    public:
        virtual const char *
        GetName() const { return mName; }

        virtual const ShaderResourceDesc &
        GetDesc() const { return mDesc; }

    public:
        ShaderResource( const char * name, const ShaderResourceDesc & desc ) { mName = name; mDesc = desc; }

    private:
        const char *        mName;
        ShaderResourceDesc  mDesc;
    };
    SHARED_PTR_TYPEDEFS( ShaderResource );

    class ShaderParameter : public IShaderParameter
    {
    public:
        virtual const ShaderParameterDesc &
        GetDesc() const { return mDesc; }

    public:
        ShaderParameter( const ShaderParameterDesc & desc ) { mDesc = desc; }

    private:
        ShaderParameterDesc mDesc;
    };
    SHARED_PTR_TYPEDEFS( ShaderParameter );

    template < class Base >
    class AShaderReflection : public ADeviceChild<Base>
    {
    public:
        virtual const ShaderDesc &
        GetDesc() const { return mDesc; }

        virtual ConstIShaderVariableRef
        GetVariable( uint32_t index ) const;

        virtual ConstIShaderVariableRef
        GetVariable( const char * name ) const;

        virtual ConstIShaderBufferRef
        GetBuffer( uint32_t index ) const;

        virtual ConstIShaderBufferRef
        GetBuffer( const char * name ) const;

        virtual ConstIShaderResourceRef
        GetResource( uint32_t index ) const;

        virtual ConstIShaderResourceRef
        GetResource( const char * name ) const;

        virtual ConstIShaderParameterRef
        GetInputParameter( uint32_t index ) const;

        virtual ConstIShaderParameterRef
        GetInputParameter( Semantic ) const;

    public:
        void Initialize();

    public:
        AShaderReflection( DeviceIn  );
        ~AShaderReflection();

    protected:
        typedef std::vector < ShaderVariableRef >   ShaderVariableVec;
        typedef std::vector < ShaderBufferRef >     ShaderBufferVec;
        typedef std::vector < ShaderResourceRef >   ShaderResourceVec;
        typedef std::vector < ShaderParameterRef >  ShaderParameterVec;

        typedef std::unordered_map < const char *, ShaderVariableRef >  ShaderVariableMap;
        typedef std::unordered_map < const char *, ShaderBufferRef >    ShaderBufferMap;
        typedef std::unordered_map < const char *, ShaderResourceRef >  ShaderResourceMap;
        typedef std::unordered_map < uint32_t, ShaderParameterRef >     ShaderParameterMap;

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

#endif // __GFW_RUNTIME_COMMON_SHADER_REFLECT_H__
