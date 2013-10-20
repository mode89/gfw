#include "common/trace.h"

#include "gfw/base/buffer.h"
#include "gfw/base/device.h"
#include "gfw/base/draw_params.h"
#include "gfw/base/input_layout.h"

#include "gfw/runtime/common/effect.h"
#include "gfw/runtime/common/factory.h"
#include "gfw/runtime/common/mesh_builder.h"
#include "gfw/runtime/common/shader_stage.h"

#include <fstream>
#include <string>

namespace GFW {

    using namespace Common;

    IFactoryRef CreateFactory(IDeviceRef device)
    {
        Factory * factory = new Factory(device);
        if (!factory->Initialize())
        {
            TRACE_FAIL_MSG("Failed to initialize a factory");
            delete factory;
            return NULL;
        }
        return factory;
    }

    Factory::Factory(IDeviceRef device)
        : mDevice(device)
    {

    }

    Factory::~Factory()
    {

    }

    bool Factory::Initialize()
    {
        return true;
    }

    IEffectRef Factory::CreateEffect(const char * fileName)
    {
        TRACE_ASSERT(fileName != NULL);

        std::ifstream file(fileName, std::ios_base::binary);
        TRACE_ASSERT(file);

        // Determine size

        file.seekg(0, file.end);
        uint64_t fileSize = file.tellg();
        file.seekg(0, file.beg);

        // Read the effect file

        Common::AutoPointer<char> effectData = new char [static_cast<uint32_t>(fileSize + 1)];
        file.read(effectData, fileSize);
        effectData[fileSize] = 0;

        // Create shaders

        IShaderRef shaders[SHADER_STAGE_COUNT];

        for (int stage = 0; stage < SHADER_STAGE_COUNT; ++ stage)
        {
            std::string shaderData = "#define GFW_SHADER_STAGE_";
            shaderData += GetStageString(static_cast<ShaderStage>(stage));
            shaderData += "\n";
            shaderData += effectData;

            shaders[stage] = mDevice->CreateShader(static_cast<ShaderStage>(stage), shaderData.c_str());
            TRACE_ASSERT(shaders[stage].IsAttached());
        }

        EffectRef effect = new Effect(shaders, mDevice);
        return effect.StaticCast<IEffect>();
    }

    IMeshBuilderRef Factory::CreateMeshBuilder()
    {
        return new MeshBuilder();
    }

} // namespace GFW
