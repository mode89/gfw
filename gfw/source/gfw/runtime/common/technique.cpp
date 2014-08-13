#include "gfw/runtime/common/pass.h"
#include "gfw/runtime/common/technique.h"
#include "gfw/shared/pass.h"
#include "gfw/shared/technique.h"

namespace GFW {

    Technique::Technique( const TechniqueBinary & binary, ShaderTable & shaderTable, IDeviceIn device )
    {
        mDesc.passCount = static_cast< uint32_t >( binary.mPasses.size() );
        mPasses.reserve( mDesc.passCount );
        for ( auto passBinary : binary.mPasses )
        {
            mPasses.push_back( std::make_shared<Pass>( passBinary, shaderTable, device ) );
        }
    }

    Technique::~Technique()
    {

    }

    void Technique::Dispatch( uint32_t pass ) const
    {
        mPasses[ pass ]->Dispatch();
    }

    ConstIShaderRef Technique::GetShader( ShaderStage stage, uint32_t pass ) const
    {
        return mPasses[pass]->GetShader( stage );
    }

} // namespace GFW
