#ifndef __GFW_GRAPHICS_BASE_CLEAR_PARAMS_H__
#define __GFW_GRAPHICS_BASE_CLEAR_PARAMS_H__

namespace GFW {

    const uint32_t CLEAR_COLOR = (1 << 0);

    struct ClearParams
    {
        uint32_t mask;
        float    color[4];

        ClearParams()
            : mask(0)
        {
            color[0] = 0.0f;
            color[1] = 0.0f;
            color[2] = 0.0f;
            color[3] = 0.0f;
        }
    };

} // namespace GFW

#endif // __GFW_GRAPHICS_BASE_CLEAR_PARAMS_H__
