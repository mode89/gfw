#include "gfw/core/device.h"
#include "gfw/core/functions.h"
#include "gfw/core/shader_reflect.h"
#include "gfw/common/shader_reflect.inl"

namespace GFW {

#define VAR_TYPES \
    T(GL_FLOAT,         SHADER_VAR_TYPE_FLOAT)      \
    T(GL_FLOAT_VEC2,    SHADER_VAR_TYPE_FLOAT2)     \
    T(GL_FLOAT_VEC3,    SHADER_VAR_TYPE_FLOAT3)     \
    T(GL_FLOAT_VEC4,    SHADER_VAR_TYPE_FLOAT4)     \
    T(GL_FLOAT_MAT4,    SHADER_VAR_TYPE_FLOAT44)    \
    T(GL_INT,           SHADER_VAR_TYPE_INT)        \
    T(GL_INT_VEC2,      SHADER_VAR_TYPE_INT2)       \
    T(GL_INT_VEC3,      SHADER_VAR_TYPE_INT3)       \
    T(GL_INT_VEC4,      SHADER_VAR_TYPE_INT4)       \
    T(GL_BOOL,          SHADER_VAR_TYPE_BOOL)       \
    T(GL_BOOL_VEC2,     SHADER_VAR_TYPE_BOOL2)      \
    T(GL_BOOL_VEC3,     SHADER_VAR_TYPE_BOOL3)      \
    T(GL_BOOL_VEC4,     SHADER_VAR_TYPE_BOOL4)      \

    static bool IsVariableType(uint32_t type)
    {
        switch (type)
        {
#define T(glType, gfwType) case glType : return true;
            VAR_TYPES
#undef T
        }

        return false;
    }

    static ShaderVariableType GetVariableType(uint32_t type)
    {
        switch (type)
        {
#define T(glType, gfwType) case glType: return gfwType;
            VAR_TYPES
#undef T
        default:
            TRACE_FAIL_MSG("Undefined variable type");
        }

        return SHADER_VAR_TYPE_UNKNOWN;
    }

    ShaderReflection::ShaderReflection(uint32_t program, IDeviceIn device)
        : AShaderReflection(device)
    {
        DeviceRef deviceImpl = device;

        char *  name          = NULL;
        int32_t maxNameLength = 0;

        int32_t uniformsCount = -1;
        TRACE_ASSERT_GL(glGetProgramInterfaceiv, program, GL_UNIFORM, GL_ACTIVE_RESOURCES, &uniformsCount);
        TRACE_ASSERT(uniformsCount != -1);

        TRACE_ASSERT_GL(glGetProgramInterfaceiv, program, GL_UNIFORM, GL_MAX_NAME_LENGTH, &maxNameLength);
        TRACE_ASSERT(uniformsCount ? (maxNameLength != 0) : 1);

        if (uniformsCount)
        {
            name = new char [maxNameLength + 1];

            for (int32_t i  = 0; i < uniformsCount; ++ i)
            {
                TRACE_ASSERT_GL(glGetProgramResourceName, program, GL_UNIFORM, i, maxNameLength + 1, NULL, name);
                const char * uniformName = deviceImpl->GetStringTable().Resolve(name);

                uint32_t props[] = {
                    GL_TYPE,
                    GL_LOCATION,
                    GL_BLOCK_INDEX,
                    GL_OFFSET
                };
                int32_t params[4];
                TRACE_ASSERT_GL(glGetProgramResourceiv, program, GL_UNIFORM, i, sizeof(props) / sizeof(props[0]),
                    props, sizeof(params), NULL, params);

                if (IsVariableType(params[0]))
                {
                    ShaderVariableDesc varDesc;
                    varDesc.type     = GetVariableType(params[0]);
                    varDesc.location = params[1];
                }
            }

            delete name;
        }
    }

    ShaderReflection::~ShaderReflection()
    {

    }

} // namespace GFW
