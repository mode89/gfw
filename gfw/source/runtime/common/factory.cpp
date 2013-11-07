#include "common/trace.h"

#include "gfw/base/buffer.h"
#include "gfw/base/device.h"
#include "gfw/base/draw_params.h"
#include "gfw/base/input_layout.h"

#include "gfw/runtime/common/effect.h"
#include "gfw/runtime/common/factory.h"
#include "gfw/runtime/common/mesh_builder.h"
#include "gfw/runtime/common/shader_stage.h"

#include "gfw/shared/effect.h"

#include "serialization/input_archive.h"

#include <fstream>
#include <string>

namespace GFW {

    using namespace Common;
    using namespace Serialization;

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

    IEffectRef Factory::CreateEffect( const char * fileName )
    {
        TRACE_ASSERT( fileName != NULL );

        std::ifstream fxStream( fileName, std::ios_base::in | std::ios_base::binary );
        TRACE_ASSERT( fxStream );

        InputArchive<std::ifstream> archive( fxStream );

        EffectBinaryRef effectBinary;
        archive & CreateNamedValue( "", effectBinary );

        TRACE_FAIL_MSG( "Not yet implemented" );

        return NULL;
    }

    IMeshBuilderRef Factory::CreateMeshBuilder()
    {
        return new MeshBuilder();
    }

} // namespace GFW
