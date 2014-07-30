#ifndef __GFW_BASE_CLEAR_PARAMS_H__
#define __GFW_BASE_CLEAR_PARAMS_H__

namespace GFW {

	enum
	{
		CLEAR_COLOR = (1 << 0),
		CLEAR_DEPTH = (1 << 1)
	};

    struct ClearParams
    {
        uint32_t mask;
        float    color[4];
		float	 depth;

        ClearParams()
            : mask(0)
			, depth(1.0f)
        {
            color[0] = 0.0f;
            color[1] = 0.0f;
            color[2] = 0.0f;
            color[3] = 0.0f;
        }
    };

} // namespace GFW

#endif // __GFW_BASE_CLEAR_PARAMS_H__
