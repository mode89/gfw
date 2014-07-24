#pragma warning( push )
#pragma warning( disable : 4242 4244 4265 4365 4619 4625 4626 4640 )
#include "boost/archive/binary_iarchive.hpp"
#include "boost/serialization/list.hpp"
#include "boost/serialization/vector.hpp"
#pragma warning( pop )

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
#include "gfw/shared/pass.h"
#include "gfw/shared/shader.h"
#include "gfw/shared/technique.h"
#include <fstream>
#include <string>

namespace GFW {

    IFactoryRef CreateFactory( IDeviceIn device )
    {
        return std::make_shared<Factory>( device );
    }

    Factory::Factory( IDeviceIn device )
        : mDevice( device )
    {

    }

    Factory::~Factory()
    {

    }

    IEffectRef Factory::CreateEffect( const char * fileName )
    {
        TRACE_ASSERT( fileName != NULL );

        std::ifstream fxStream( fileName, std::ios_base::in | std::ios_base::binary );
        TRACE_ASSERT( fxStream );

        boost::archive::binary_iarchive archive( fxStream );

        EffectBinary effectBinary;
        archive >> effectBinary;

        return std::make_shared<Effect>( effectBinary, mDevice.lock() );
    }

    IMeshBuilderRef Factory::CreateMeshBuilder()
    {
        return std::make_shared<MeshBuilder>();
    }

} // namespace GFW
