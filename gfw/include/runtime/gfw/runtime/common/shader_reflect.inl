#ifndef __GFW_RUNTIME_COMMON_SHADER_REFLECT_INL__
#define __GFW_RUNTIME_COMMON_SHADER_REFLECT_INL__

#include "common/trace.h"
#include "gfw/runtime/common/device_child.inl"

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
    void AShaderReflection<Base>::Initialize()
    {
        for (uint32_t i = 0; i < mDesc.inputsCount; ++ i)
        {
            const ShaderParameterDesc & paramDesc = mInputs[i]->GetDesc();
            mInputsMap[paramDesc.semantic] = mInputs[i];
        }

        for (uint32_t i = 0; i < mDesc.variableCount; ++ i)
        {
            ShaderVariableRef var = mVariables[i];
            mVariablesMap[var->GetName()] = var;
        }

        for (uint32_t i = 0; i < mDesc.bufferCount; ++ i)
        {
            ShaderBufferRef buf = mBuffers[i];
            mBuffersMap[buf->GetName()] = buf;
        }

        for (uint32_t i = 0; i < mDesc.resourceCount; ++ i)
        {
            ShaderResourceRef res = mResources[i];
            mResourcesMap[res->GetName()] = res;
        }
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
    IShaderParameterRef AShaderReflection<Base>::GetInputParameter(Semantic semantic)
    {
        ShaderParameterMap::iterator it = mInputsMap.find(semantic);
        TRACE_ASSERT(it != mInputsMap.end());
        return it->second;
    }

} // namespace GFW

#endif // __GFW_RUNTIME_COMMON_SHADER_REFLECT_INL__
