#ifndef __GFW_BASE_SHADER_STAGE_H__
#define __GFW_BASE_SHADER_STAGE_H__

namespace GFW {

    enum ShaderStage : int32_t
    {
        UNKNOWN = -1,
        VERTEX,
        PIXEL,
        COUNT,
    };

} // namespace GFW

#endif // __GFW_BASE_SHADER_STAGE_H__
