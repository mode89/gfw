#ifndef __GFW_GRAPHICS_BASE_SHADER_H__
#define __GFW_GRAPHICS_BASE_SHADER_H__

#include "common/autoref.h"

namespace GFW {

    enum ShaderStage
    {
        SHADER_UNKNOWN = -1,
        SHADER_VERTEX,
        SHADER_PIXEL,
        SHADER_STAGE_NUMBER
    };

    class IShader: public Common::ARefCounted
    {
    public:
        virtual ShaderStage     GetStage() = 0;
    };
    AUTOREF_REFERENCE_DECLARATION(IShader);

} // namespace GFW

#endif // __GFW_GRAPHICS_BASE_SHADER_H__
