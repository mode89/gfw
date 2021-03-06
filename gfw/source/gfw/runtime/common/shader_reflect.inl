#ifndef __GFW_RUNTIME_COMMON_SHADER_REFLECT_INL__
#define __GFW_RUNTIME_COMMON_SHADER_REFLECT_INL__

#include "cmn/trace.h"
#include "gfw/runtime/common/device_child.inl"

namespace GFW {

    template < class Base >
    AShaderReflection< Base >::AShaderReflection( DeviceIn device )
        : ADeviceChild< Base >( device )
    {

    }

    template < class Base >
    AShaderReflection< Base >::~AShaderReflection()
    {

    }

    template < class Base >
    void AShaderReflection< Base >::Initialize()
    {
        for ( unsigned i = 0; i < mDesc.inputsCount; ++ i )
        {
            const ShaderParameterDesc & paramDesc = mInputs[ i ]->GetDesc();
            mInputsMap[ paramDesc.semantic ] = mInputs[ i ];
        }

        for ( unsigned i = 0; i < mDesc.variableCount; ++ i )
        {
            ShaderVariableRef var = mVariables[i];
            mVariablesMap[ var->GetName() ] = var;
        }

        for ( unsigned i = 0; i < mDesc.bufferCount; ++ i )
        {
            ShaderBufferRef buf = mBuffers[i];
            mBuffersMap[ buf->GetName() ] = buf;
        }

        for ( unsigned i = 0; i < mDesc.resourceCount; ++ i )
        {
            ShaderResourceRef res = mResources[i];
            mResourcesMap[ res->GetName() ] = res;
        }
    }

    template < class Base >
    ConstIShaderVariableRef AShaderReflection<Base>::GetVariable( unsigned index ) const
    {
        CMN_ASSERT( index < mDesc.variableCount );
        return mVariables[ index ];
    }

    template < class Base >
    ConstIShaderVariableRef AShaderReflection<Base>::GetVariable( const char * name ) const
    {
        ShaderVariableMap::const_iterator it = mVariablesMap.find( name );
        CMN_ASSERT( it != mVariablesMap.end() );
        return it->second;
    }

    template < class Base >
    ConstIShaderBufferRef AShaderReflection<Base>::GetBuffer( unsigned index ) const
    {
        CMN_ASSERT( index < mDesc.bufferCount );
        return mBuffers[ index ];
    }

    template < class Base >
    ConstIShaderBufferRef AShaderReflection<Base>::GetBuffer( const char * name ) const
    {
        ShaderBufferMap::const_iterator it = mBuffersMap.find( name );
        CMN_ASSERT( it != mBuffersMap.end() );
        return it->second;
    }

    template < class Base >
    ConstIShaderResourceRef AShaderReflection<Base>::GetResource( unsigned index ) const
    {
        CMN_ASSERT( index < mDesc.resourceCount );
        return mResources[ index ];
    }

    template < class Base >
    ConstIShaderResourceRef AShaderReflection<Base>::GetResource( const char * name ) const
    {
        ShaderResourceMap::const_iterator it = mResourcesMap.find( name );
        CMN_ASSERT( it != mResourcesMap.end() );
        return it->second;
    }

    template < class Base >
    ConstIShaderParameterRef AShaderReflection<Base>::GetInputParameter( unsigned index ) const
    {
        CMN_ASSERT( index < mDesc.inputsCount );
        return mInputs[ index ];
    }

    template < class Base >
    ConstIShaderParameterRef AShaderReflection<Base>::GetInputParameter( Semantic semantic ) const
    {
        ShaderParameterMap::const_iterator it = mInputsMap.find( semantic );
        CMN_ASSERT( it != mInputsMap.end() );
        return it->second;
    }

} // namespace GFW

#endif // __GFW_RUNTIME_COMMON_SHADER_REFLECT_INL__
