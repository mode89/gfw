#include "cmn/platform.h"

CMN_WARNING_PUSH
CMN_WARNING_DISABLE_MSVC( 4242 4244 4265 4365 4619 4625 4626 4640 )
CMN_WARNING_DISABLE_GCC( unused-local-typedefs )
#include "boost/archive/binary_iarchive.hpp"
#include "boost/serialization/list.hpp"
#include "boost/serialization/vector.hpp"
CMN_WARNING_POP

#include "cmn/trace.h"
#include "gfw/buffer.h"
#include "gfw/device.h"
#include "gfw/draw_params.h"
#include "gfw/input_layout.h"
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

    AFactory::AFactory()
    {

    }

    AFactory::~AFactory()
    {

    }

    IEffectRef AFactory::CreateEffect( IDeviceIn device, const char * fileName )
    {
        CMN_ASSERT( fileName != NULL );

        std::ifstream fxStream( fileName, std::ios_base::in | std::ios_base::binary );
        CMN_ASSERT( fxStream );

        boost::archive::binary_iarchive archive( fxStream );

        EffectBinary effectBinary;
        archive >> effectBinary;

        return std::make_shared< Effect >( effectBinary, device );
    }

    IMeshBuilderRef AFactory::CreateMeshBuilder()
    {
        return std::make_shared< MeshBuilder >();
    }

} // namespace GFW
