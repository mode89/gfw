#ifndef __GFW_SHARED_SHADER_REFLECT_H__
#define __GFW_SHARED_SHADER_REFLECT_H__

#include "common/typedefs.h"

#include "gfw/shared/semantic.h"

namespace GFW {

    struct ShaderDesc
    {
        uint32_t    variableCount;
        uint32_t    bufferCount;
        uint32_t    resourceCount;
        uint32_t    inputsCount;

        ShaderDesc()
            : variableCount(0)
            , bufferCount(0)
            , resourceCount(0)
            , inputsCount(0)
        {}
    };

    enum ShaderVariableType
    {
        SHADER_VAR_TYPE_UNKNOWN = 0,
        SHADER_VAR_TYPE_FLOAT,
        SHADER_VAR_TYPE_FLOAT2,
        SHADER_VAR_TYPE_FLOAT3,
        SHADER_VAR_TYPE_FLOAT4,
        SHADER_VAR_TYPE_FLOAT44,
        SHADER_VAR_TYPE_INT,
        SHADER_VAR_TYPE_INT2,
        SHADER_VAR_TYPE_INT3,
        SHADER_VAR_TYPE_INT4,
        SHADER_VAR_TYPE_BOOL,
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

    struct ShaderBufferDesc
    {
        uint32_t    size;

        ShaderBufferDesc()
            : size(0)
        {}
    };

    enum ShaderResourceType
    {
        SHADER_RES_TYPE_UNKNOWN = 0,
        SHADER_RES_TYPE_CBUFFER,
        SHADER_RES_TYPE_TEXTURE,
        SHADER_RES_TYPE_BUFFER
    };

    enum ShaderResourceDim
    {
        SHADER_RES_DIM_UNKNOWN = 0,
        SHADER_RES_DIM_BUFFER,
        SHADER_RES_DIM_TEX_2D,
        SHADER_RES_DIM_TEX_2D_MSAA,
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

    struct ShaderParameterDesc
    {
        Semantic    semantic;
        uint32_t    location;

        ShaderParameterDesc()
            : semantic(SEMANTIC_UNKNOWN)
            , location(0)
        {}
    };

} // namespace GFW

#endif // __GFW_SHARED_SHADER_REFLECT_H__
