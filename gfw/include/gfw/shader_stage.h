#ifndef __GFW_BASE_SHADER_STAGE_H__
#define __GFW_BASE_SHADER_STAGE_H__

namespace GFW {

    enum ShaderStage : int32_t
    {
        SHADER_STAGE_UNKNOWN = -1,
        SHADER_STAGE_VERTEX,
        SHADER_STAGE_PIXEL,
        SHADER_STAGE_COUNT,
    };

} // namespace GFW

#endif // __GFW_BASE_SHADER_STAGE_H__
