#include "gfw/core/device.h"
#include "gfw/core/functions.h"
#include "gfw/core/shader_reflect.h"
#include "gfw/common/shader_reflect.inl"

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
            TRACE_FAIL_MSG("Undefined variable type");
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
            TRACE_FAIL_MSG("Undefined variable type");
        }

        return 0;
    }

    ShaderReflection::ShaderReflection(uint32_t program, IDeviceIn device)
        : AShaderReflection(device)
    {
        DeviceRef deviceImpl = device;

        char *  name          = NULL;
        int32_t maxNameLength = 0;

        int32_t uniformBlocksCount = -1;
        TRACE_ASSERT_GL(glGetProgramInterfaceiv, program, GL_UNIFORM_BLOCK, GL_ACTIVE_RESOURCES, &uniformBlocksCount);
        TRACE_ASSERT(uniformBlocksCount != -1);

        TRACE_ASSERT_GL(glGetProgramInterfaceiv, program, GL_UNIFORM_BLOCK, GL_MAX_NAME_LENGTH, &maxNameLength);
        TRACE_ASSERT(uniformBlocksCount ? (maxNameLength != 0) : 1);

        if (uniformBlocksCount)
        {
            name = new char [maxNameLength + 1];

            for (int32_t i = 0; i < uniformBlocksCount; ++ i)
            {
                TRACE_ASSERT_GL(glGetProgramResourceName, program, GL_UNIFORM_BLOCK, i, maxNameLength + 1, NULL, name);
                const char * uniformBlockName = deviceImpl->GetStringTable().Resolve(name);

                uint32_t props[] = {
                    GL_BUFFER_DATA_SIZE
                };
                int32_t params;
                TRACE_ASSERT_GL(glGetProgramResourceiv, program, GL_UNIFORM_BLOCK, i, sizeof(props) / sizeof(props[0]),
                    props, sizeof(params), NULL, &params);

                ShaderBufferDesc bufDesc;
                bufDesc.size = params;

                mBuffers.push_back(new ShaderBuffer(uniformBlockName, bufDesc));
                mDesc.bufferCount ++;
            }

            delete [] name;
        }

        int32_t uniformsCount = -1;
        TRACE_ASSERT_GL(glGetProgramInterfaceiv, program, GL_UNIFORM, GL_ACTIVE_RESOURCES, &uniformsCount);
        TRACE_ASSERT(uniformsCount != -1);

        TRACE_ASSERT_GL(glGetProgramInterfaceiv, program, GL_UNIFORM, GL_MAX_NAME_LENGTH, &maxNameLength);
        TRACE_ASSERT(uniformsCount ? (maxNameLength != 0) : 1);

        if (uniformsCount)
        {
            name = new char [maxNameLength + 1];

            for (int32_t i = 0; i < uniformsCount; ++ i)
            {
                TRACE_ASSERT_GL(glGetProgramResourceName, program, GL_UNIFORM, i, maxNameLength + 1, NULL, name);
                const char * uniformName = deviceImpl->GetStringTable().Resolve(name);

                uint32_t props[] = {
                    GL_TYPE,
                    GL_BLOCK_INDEX,
                    GL_LOCATION,
                    GL_OFFSET,
                    GL_ARRAY_SIZE,
                    GL_ARRAY_STRIDE
                };
                int32_t params[6];
                TRACE_ASSERT_GL(glGetProgramResourceiv, program, GL_UNIFORM, i, sizeof(props) / sizeof(props[0]),
                    props, sizeof(params), NULL, params);

                if (IsVariableType(params[0]))
                {
                    ShaderVariableDesc varDesc;
                    varDesc.type        = GetVariableType(params[0]);
                    varDesc.bufferIndex = params[1];
                    varDesc.location    = (params[1] != -1) ? params[3] : params[2];

                    uint32_t typeSize   = GetVariableTypeSize(params[0]);
                    uint32_t stride     = (params[1] != -1) ? params[5] : typeSize;
                    varDesc.size        = (params[4] == 0) ? typeSize : params[4] * stride;

                    mVariables.push_back(new ShaderVariable(uniformName, varDesc));
                    mDesc.variableCount ++;
                }
            }

            delete [] name;
        }
    }

    ShaderReflection::~ShaderReflection()
    {

    }

} // namespace GFW
