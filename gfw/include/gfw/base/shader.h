#ifndef __GFW_BASE_SHADER_H__
#define __GFW_BASE_SHADER_H__

#include "gfw/base/types_fwd.h"

namespace GFW {

    enum ShaderStage
    {
        SHADER_UNKNOWN = -1,
        SHADER_VERTEX,
        SHADER_PIXEL,
        SHADER_STAGE_COUNT
    };

    class IShader: public Common::ARefCounted
    {
    public:
        virtual int32_t
        GetStage() = 0;
    };
    AUTOREF_REFERENCE_DECLARATION(IShader);

} // namespace GFW

#endif // __GFW_BASE_SHADER_H__
