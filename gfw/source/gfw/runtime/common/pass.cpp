#include "cmn/trace.h"
#include "gfw/device.h"
#include "gfw/context.h"
#include "gfw/runtime/common/pass.h"
#include "gfw/runtime/ogl4/device.h"
#include "gfw/shared/pass.h"

namespace GFW {

    Pass::Pass( const PassBinary & binary, const ShaderTable & shaderMap, IDeviceIn device )
        : mDevice( device )
    {
        for ( uint32_t stage = 0; stage < SHADER_STAGE_COUNT; ++ stage )
        {
            const ShaderBinary * shaderBinary = binary.mShaders[ stage ];
            ShaderTable::const_iterator shaderIt = shaderMap.find( shaderBinary );
            CMN_ASSERT( shaderIt != shaderMap.end() );
            mShaders[ stage ] = shaderIt->second;
        }
    }

    Pass::~Pass()
    {

    }

    void Pass::Dispatch() const
    {
        IContextRef context = IDeviceRef( mDevice )->GetCurrentContext();
        CMN_ASSERT( context );

        for ( uint32_t stage = 0; stage< SHADER_STAGE_COUNT; ++ stage )
        {
            context->SetShader( static_cast<ShaderStage>( stage ), mShaders[stage] );
        }
    }

} // namespace GFW
