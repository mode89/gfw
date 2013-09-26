#ifndef __GFW_COMMON_SHADER_REFLECT_INL__
#define __GFW_COMMON_SHADER_REFLECT_INL__

#include "common/trace.h"
#include "gfw/common/device_child.inl"

namespace GFW {

    template < class Base >
    AShaderReflection<Base>::AShaderReflection(IDeviceIn device)
        : ADeviceChild<Base>(device)
    {

    }

    template < class Base >
    AShaderReflection<Base>::~AShaderReflection()
    {

    }

    template < class Base >
    void AShaderReflection<Base>::Initialize(const ShaderDesc & desc)
    {
        mDesc = desc;
        mVariables.resize(mDesc.variableCount);
        mBuffers.resize(mDesc.bufferCount);
        mResources.resize(mDesc.resourceCount);
        mInputs.resize(mDesc.inputsCount);
    }

    template < class Base >
    IShaderVariableRef AShaderReflection<Base>::GetVariable(uint32_t index)
    {
        TRACE_ASSERT(index < mDesc.variableCount);
        return mVariables[index];
    }

    template < class Base >
    IShaderVariableRef AShaderReflection<Base>::GetVariable(const char * name)
    {
        ShaderVariableMap::iterator it = mVariablesMap.find(name);
        TRACE_ASSERT(it != mVariablesMap.end());
        return it->second;
    }

    template < class Base >
    IShaderBufferRef AShaderReflection<Base>::GetBuffer(uint32_t index)
    {
        TRACE_ASSERT(index < mDesc.bufferCount);
        return mBuffers[index];
    }

    template < class Base >
    IShaderBufferRef AShaderReflection<Base>::GetBuffer(const char * name)
    {
        ShaderBufferMap::iterator it = mBuffersMap.find(name);
        TRACE_ASSERT(it != mBuffersMap.end());
        return it->second;
    }

    template < class Base >
    IShaderResourceRef AShaderReflection<Base>::GetResource(uint32_t index)
    {
        TRACE_ASSERT(index < mDesc.resourceCount);
        return mResources[index];
    }

    template < class Base >
    IShaderResourceRef AShaderReflection<Base>::GetResource(const char * name)
    {
        ShaderResourceMap::iterator it = mResourcesMap.find(name);
        TRACE_ASSERT(it != mResourcesMap.end());
        return it->second;
    }

    template < class Base >
    IShaderParameterRef AShaderReflection<Base>::GetInputParameter(uint32_t index)
    {
        TRACE_ASSERT(index < mDesc.inputsCount);
        return mInputs[index];
    }

    template < class Base >
    IShaderParameterRef AShaderReflection<Base>::GetInputParameter(const char * name)
    {
        ShaderParameterMap::iterator it = mInputsMap.find(name);
        TRACE_ASSERT(it != mInputsMap.end());
        return it->second;
    }

} // namespace GFW

#endif // __GFW_COMMON_SHADER_REFLECT_INL__
