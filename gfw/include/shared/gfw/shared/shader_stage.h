#ifndef __GFW_SHARED_SHADER_STAGE_H__
#define __GFW_SHARED_SHADER_STAGE_H__

namespace GFW {

    enum ShaderStage : int32_t
    {
        UNKNOWN = -1,
        VERTEX,
        PIXEL,
        COUNT,
    };

} // namespace GFW

#endif // __GFW_SHARED_SHADER_STAGE_H__
