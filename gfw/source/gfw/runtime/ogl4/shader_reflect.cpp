#include "gfw/runtime/ogl4/device.h"
#include "gfw/runtime/ogl4/functions.h"
#include "gfw/runtime/ogl4/limits.h"
#include "gfw/runtime/ogl4/shader_reflect.h"
#include "gfw/runtime/common/shader_reflect.inl"
#include "algorithm"

namespace GFW {

#define VAR_TYPES \
    T(GL_FLOAT,         SHADER_VAR_TYPE_FLOAT,      4)      \
    T(GL_FLOAT_VEC2,    SHADER_VAR_TYPE_FLOAT2,     8)      \
    T(GL_FLOAT_VEC3,    SHADER_VAR_TYPE_FLOAT3,     12)     \
    T(GL_FLOAT_VEC4,    SHADER_VAR_TYPE_FLOAT4,     16)     \
    T(GL_FLOAT_MAT4,    SHADER_VAR_TYPE_FLOAT44,    64)     \
    T(GL_INT,           SHADER_VAR_TYPE_INT,        4)      \
    T(GL_INT_VEC2,      SHADER_VAR_TYPE_INT2,       8)      \
    T(GL_INT_VEC3,      SHADER_VAR_TYPE_INT3,       12)     \
    T(GL_INT_VEC4,      SHADER_VAR_TYPE_INT4,       16)     \
    T(GL_BOOL,          SHADER_VAR_TYPE_BOOL,       4)      \

#define SAMPLER_TYPES \
    T(GL_SAMPLER_2D,                SHADER_RES_DIM_TEX_2D)      \
    T(GL_SAMPLER_2D_MULTISAMPLE,    SHADER_RES_DIM_TEX_2D_MSAA) \

    static bool IsVariableType(uint32_t type)
    {
        switch (type)
        {
#define T(glType, gfwType, size) case glType : return true;
            VAR_TYPES
#undef T
        }

        return false;
    }

    static ShaderVariableType GetVariableType(uint32_t type)
    {
        switch (type)
        {
#define T(glType, gfwType, size) case glType: return gfwType;
            VAR_TYPES
#undef T
        default:
            CMN_FAIL_MSG( "Undefined variable type" );
        }

        return SHADER_VAR_TYPE_UNKNOWN;
    }

    static uint32_t GetVariableTypeSize(uint32_t type)
    {
        switch (type)
        {
#define T(glType, gfwType, size) case glType : return size;
            VAR_TYPES
#undef T
        default:
            CMN_FAIL_MSG( "Undefined variable type" );
        }

        return 0;
    }

    static bool IsSamplerType(uint32_t type)
    {
        switch (type)
        {
#define T(glType, dim) case glType:
            SAMPLER_TYPES
                return true;
#undef T
        }

        return false;
    }

    static ShaderResourceDim GetSamplerDim(uint32_t type)
    {
        switch (type)
        {
#define T(glType, dim) case glType : return dim;
            SAMPLER_TYPES
#undef T
        default:
            CMN_FAIL_MSG( "Undefined type of the sampler" );
        }

        return SHADER_RES_DIM_UNKNOWN;
    }

    ShaderReflection::ShaderReflection(
        const ShaderBinary & shaderBinary,
        const ShaderBinaryOgl4 & shaderBinaryOgl4,
        uint32_t program,
        DeviceIn device )
        : AShaderReflection( device )
        , mInputSignature( 0 )
    {
        DeviceRef deviceImpl = device;

        char name[128];

        // Reflect input parameters

            int32_t inputsCount = -1;
            VGL( glGetProgramInterfaceiv, program, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &inputsCount );
            CMN_ASSERT( inputsCount != -1 );

            for (int32_t i = 0; i < inputsCount; ++ i)
            {
                VGL( glGetProgramResourceName, program, GL_PROGRAM_INPUT, i, sizeof(name), NULL, name );

                uint32_t props[] = {
                    GL_TYPE,
                    GL_LOCATION,
                };
                const uint32_t propsCount = sizeof(props) / sizeof(props[0]);
                int32_t params[propsCount];
                VGL( glGetProgramResourceiv, program, GL_PROGRAM_INPUT, i, propsCount, props, sizeof(params), NULL, params );

                ShaderParameterDesc paramDesc;
                paramDesc.location = params[1];
                paramDesc.semantic = GetSemantic(name);

                mInputs.push_back( std::make_shared<ShaderParameter>( paramDesc ) );
                mDesc.inputsCount ++;
            }

        // Calculate input signature

            mInputSignature = 0;
            ShaderParameterVec sortedInputs = mInputs;
            std::sort( sortedInputs.begin(), sortedInputs.end(),
                [] ( const ShaderParameterRef & p1, const ShaderParameterRef & p2 ) -> bool {
                    return p1->GetDesc().semantic < p2->GetDesc().semantic;
                } );
            for ( auto & par : sortedInputs )
            {
                std::string parString( reinterpret_cast< const char * >( &par->GetDesc() ),
                    sizeof( ShaderParameterDesc ) );
                mInputSignature ^= std::hash< std::string >()( parString );
            }

        // Reflect uniform blocks

            for ( auto & res : shaderBinary.mResources )
            {
                if ( res.type == SHADER_RES_TYPE_CBUFFER )
                {
                    int index = VGL( glGetProgramResourceIndex, program, GL_UNIFORM_BLOCK, res.name.c_str() );
                    CMN_ASSERT( index != GL_INVALID_INDEX );

                    VGL( glUniformBlockBinding, program, index,
                        shaderBinary.mStage * MAX_BIND_CBUFFERS + res.bindSlot );

                    ShaderResourceDesc resDesc;
                    resDesc.bindPoint   = res.bindSlot;
                    resDesc.bindCount   = res.bindCount;
                    resDesc.type        = static_cast< ShaderResourceType >( res.type );
                    resDesc.dim         = static_cast< ShaderResourceDim >( res.dim );
                    mResources.push_back( std::make_shared< ShaderResource >( res.name, resDesc ) );
                }
            }

        // Reflect uniforms

            int32_t uniformsCount = -1;
            VGL( glGetProgramInterfaceiv, program, GL_UNIFORM, GL_ACTIVE_RESOURCES, &uniformsCount );
            CMN_ASSERT( uniformsCount != -1 );

            for (int32_t i = 0; i < uniformsCount; ++ i)
            {
                VGL( glGetProgramResourceName, program, GL_UNIFORM, i, sizeof(name), NULL, name );

                uint32_t props[] = {
                    GL_TYPE,
                    GL_BLOCK_INDEX,
                    GL_LOCATION,
                    GL_OFFSET,
                    GL_ARRAY_SIZE,
                    GL_ARRAY_STRIDE
                };
                const uint32_t propsCount = sizeof(props) / sizeof(props[0]);
                int32_t params[propsCount];
                VGL( glGetProgramResourceiv, program, GL_UNIFORM, i, propsCount, props, sizeof(params), NULL, params );

                if (IsVariableType(params[0]))
                {
                    ShaderVariableDesc varDesc;
                    varDesc.type        = GetVariableType(params[0]);
                    varDesc.bufferIndex = params[1];
                    varDesc.location    = (params[1] != -1) ? params[3] : params[2];

                    uint32_t typeSize   = GetVariableTypeSize(params[0]);
                    uint32_t stride     = (params[1] != -1) ? params[5] : typeSize;
                    varDesc.size        = (params[4] == 0) ? typeSize : params[4] * stride;

                    mVariables.push_back( std::make_shared<ShaderVariable>( name, varDesc ) );
                    mDesc.variableCount ++;
                }
                else if (IsSamplerType(params[0]))
                {
                    ShaderResourceDesc resDesc;
                    resDesc.type = SHADER_RES_TYPE_TEXTURE;
                    resDesc.dim  = GetSamplerDim(params[0]);
                    resDesc.bindPoint = params[2];
                    resDesc.bindCount = (params[4] == 0) ? 1 : params[4];

                    mResources.push_back( std::make_shared<ShaderResource>( name, resDesc ) );
                    mDesc.resourceCount ++;
                }
                else
                {
                    CMN_FAIL_MSG( "Undefined type of the uniform" );
                }
            }

        // Reflect texture-samplers

            mTextureSamplers.reserve( shaderBinaryOgl4.mTextureSamplers.size() );
            for ( auto & binary : shaderBinaryOgl4.mTextureSamplers )
            {
                TextureSamplerLocation textureSampler;
                textureSampler.texture = binary.texture;
                textureSampler.sampler = binary.sampler;
                textureSampler.location = VGL( glGetProgramResourceLocation,
                    program, GL_UNIFORM, binary.name.c_str() );
                CMN_ASSERT( textureSampler.location != -1 );
                mTextureSamplers.push_back( textureSampler );
            }

        AShaderReflection::Initialize();
    }

    ShaderReflection::~ShaderReflection()
    {

    }

} // namespace GFW
