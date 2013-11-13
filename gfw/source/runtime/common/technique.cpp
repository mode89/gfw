#include "gfw/runtime/common/pass.h"
#include "gfw/runtime/common/technique.h"
#include "gfw/shared/technique.h"

namespace GFW {

    Technique::Technique( TechniqueBinaryRef binary )
        : mDesc( binary->mDesc )
    {
        mPasses.reserve( mDesc.passCount );
        for ( uint32_t i = 0; i < mDesc.passCount; ++ i )
        {
            PassBinaryRef passBin = binary->mPasses[i];
            PassRef pass = new Pass( passBin );
        }
    }

    Technique::~Technique()
    {

    }

    void Technique::Dispatch( uint32_t pass /* = 0 */ )
    {

    }

} // namespace GFW
