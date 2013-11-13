#ifndef __GFW_SHARED_PASS_H__
#define __GFW_SHARED_PASS_H__

#include "common/autoref.h"
#include "common/string_table.h"
#include "gfw/shared/shader_stage.h"

namespace GFW {

    struct PassDesc
    {
    };

    class PassBinary : public Common::ARefCounted
    {
    public:
        Common::InternedStringBinary    mName;
        uint32_t                        mShaders[ ShaderStage::COUNT ];

        template < class Archive > void
        Serialize( Archive & archive )
        {
            archive & NAMED_VALUE( mName );
            archive & NAMED_ARRAY( mShaders, ShaderStage::COUNT );
        }
    };
    AUTOREF_REFERENCE_DECLARATION( PassBinary );

} // namespace GFW

#endif // __GFW_SHARED_PASS_H__
