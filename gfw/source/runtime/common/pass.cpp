#include "cmn/trace.h"
#include "gfw/base/device.h"
#include "gfw/base/context.h"
#include "gfw/runtime/common/pass.h"
#include "gfw/runtime/core/device.h"
#include "gfw/shared/pass.h"

namespace GFW {

    Pass::Pass( const PassBinary & binary, const ShaderTable & shaderMap, IDeviceIn device )
        : mDevice( device )
    {
        for ( uint32_t stage = 0; stage < ShaderStage::COUNT; ++ stage )
        {
            const ShaderBinary * shaderBinary = binary.mShaders[ stage ];
            ShaderTable::const_iterator shaderIt = shaderMap.find( shaderBinary );
            TRACE_ASSERT( shaderIt != shaderMap.end() );
            mShaders[ stage ] = shaderIt->second;
        }
    }

    Pass::~Pass()
    {

    }

    void Pass::Dispatch() const
    {
        IContextRef context = IDeviceRef( mDevice )->GetCurrentContext();
        TRACE_ASSERT( context );

        for ( uint32_t stage = 0; stage< ShaderStage::COUNT; ++ stage )
        {
            context->SetShader( static_cast<ShaderStage>( stage ), mShaders[stage] );
        }
    }

} // namespace GFW
