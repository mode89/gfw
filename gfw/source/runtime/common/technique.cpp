#include "gfw/runtime/common/pass.h"
#include "gfw/runtime/common/technique.h"
#include "gfw/shared/technique.h"

namespace GFW {

    Technique::Technique( TechniqueBinaryRef binary, const Effect * effect )
        : mEffect( effect )
        , mDesc( binary->mDesc )
    {
        mPasses.reserve( mDesc.passCount );
        for ( uint32_t i = 0; i < mDesc.passCount; ++ i )
        {
            PassBinaryRef passBin = binary->mPasses[i];
            mPasses.push_back( new Pass( passBin, effect ) );
        }
    }

    Technique::~Technique()
    {

    }

    void Technique::Dispatch( uint32_t pass /* = 0 */ )
    {
        mPasses[ pass ]->Dispatch();
    }

    IShaderRef Technique::GetShader( ShaderStage stage, uint32_t pass /* = 0 */ ) const
    {
        return mPasses[pass]->GetShader( stage );
    }

} // namespace GFW
