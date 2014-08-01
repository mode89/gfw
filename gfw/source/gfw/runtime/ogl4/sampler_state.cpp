#include "cmn/trace.h"
#include "gfw/runtime/ogl4/device.h"
#include "gfw/runtime/ogl4/functions.h"
#include "gfw/runtime/ogl4/sampler_state.h"

namespace GFW {

#define FILTERS                      /* MIN         MAG         MIPMAP */ \
    F(MIN_MAG_MIP_POINT,                NEAREST,    NEAREST,    NEAREST) \
    F(MIN_MAG_POINT_MIP_LINEAR,         NEAREST,    NEAREST,    LINEAR) \
    F(MIN_POINT_MAG_LINEAR_MIP_POINT,   NEAREST,    LINEAR,     NEAREST) \
    F(MIN_POINT_MAG_MIP_LINEAR,         NEAREST,    LINEAR,     LINEAR) \
    F(MIN_LINEAR_MAG_MIP_POINT,         LINEAR,     NEAREST,    NEAREST) \
    F(MIN_LINEAR_MAG_POINT_MIP_LINEAR,  LINEAR,     NEAREST,    LINEAR) \
    F(MIN_MAG_LINEAR_MIP_POINT,         LINEAR,     LINEAR,     NEAREST) \
    F(MIN_MAG_MIP_LINEAR,               LINEAR,     LINEAR,     LINEAR)

    static int32_t GetGLMinFilter(Filter f)
    {
        switch (f)
        {
        #define F(filter, min, mag, mip) case FILTER_ ## filter: return GL_ ## min ## _MIPMAP_ ## mip;
            FILTERS
        #undef F

        #define F(filter, min, mag, mip) case FILTER_COMPARE_ ## filter: return GL_ ## min ## _MIPMAP_ ## mip;
            FILTERS
        #undef F

        default:
            CMN_FAIL();
        }

        return 0;
    }

    static int32_t GetGLMagFilter(Filter f)
    {
        switch (f)
        {
        #define F(filter, min, mag, mip) case FILTER_ ## filter: return GL_ ## mag;
            FILTERS
        #undef F

        #define F(filter, min, mag, mip) case FILTER_COMPARE_ ## filter: return GL_ ## mag;
            FILTERS
        #undef F

        default:
            CMN_FAIL();
        }

        return 0;
    }

    static int32_t GetGLAddressMode(AddressMode mode)
    {
        switch (mode)
        {
        case ADDRESSMODE_BORDER:
            return GL_CLAMP_TO_BORDER;
        case ADDRESSMODE_CLAMP:
            return GL_CLAMP_TO_EDGE;
        case ADDRESSMODE_MIRROR:
            return GL_MIRRORED_REPEAT;
        case ADDRESSMODE_WRAP:
            return GL_REPEAT;
        default:
            CMN_FAIL();
        }

        return 0;
    }

    static int32_t GetGLCompareMode(Filter f)
    {
        switch (f)
        {
        #define F(filter, min, mag, mip) case FILTER_ ## filter: return GL_NONE;
            FILTERS
        #undef F

        #define F(filter, min, mag, mip) case FILTER_COMPARE_ ## filter: return GL_COMPARE_REF_TO_TEXTURE;
            FILTERS
        #undef F

        default:
            CMN_FAIL();
        }

        return 0;
    }

    static int32_t GetGLCompareFunc(CompareFunc func)
    {
        switch (func)
        {
        case COMPARE_ALWAYS:
            return GL_ALWAYS;
        case COMPARE_EQUAL:
            return GL_EQUAL;
        case COMPARE_GREATER:
            return GL_GREATER;
        case COMPARE_GREATER_EQUAL:
            return GL_GEQUAL;
        case COMPARE_LESS:
            return GL_LESS;
        case COMPARE_LESS_EQUAL:
            return GL_LEQUAL;
        case COMPARE_NEVER:
            return GL_NEVER;
        case COMPARE_NOT_EQUAL:
            return GL_NOTEQUAL;
        default:
            CMN_FAIL();
        }

        return 0;
    }

    SamplerState::SamplerState( const SamplerStateDesc & desc, uint32_t descHash, DeviceIn device )
        : ADeviceChild( device )
        , mDesc( desc )
        , mHandle( 0 )
    {
        VGL( glGenSamplers, 1, &mHandle );

        // Set sampler's states
        VGL( glSamplerParameteri,  mHandle, GL_TEXTURE_WRAP_S,       GetGLAddressMode(desc.addressU ));
        VGL( glSamplerParameteri,  mHandle, GL_TEXTURE_WRAP_T,       GetGLAddressMode(desc.addressV ));
        VGL( glSamplerParameteri,  mHandle, GL_TEXTURE_WRAP_R,       GetGLAddressMode(desc.addressW ));

        VGL( glSamplerParameterfv, mHandle, GL_TEXTURE_BORDER_COLOR, desc.borderColor );

        VGL( glSamplerParameteri,  mHandle, GL_TEXTURE_COMPARE_MODE, GetGLCompareMode(desc.filter ));
        VGL( glSamplerParameteri,  mHandle, GL_TEXTURE_COMPARE_FUNC, GetGLCompareFunc(desc.comparison ));

        VGL( glSamplerParameteri,  mHandle, GL_TEXTURE_MIN_FILTER,   GetGLMinFilter(desc.filter ));
        VGL( glSamplerParameteri,  mHandle, GL_TEXTURE_MAG_FILTER,   GetGLMagFilter(desc.filter ));

        VGL( glSamplerParameterf,  mHandle, GL_TEXTURE_MAX_LOD,      desc.maxLod );
        VGL( glSamplerParameterf,  mHandle, GL_TEXTURE_MIN_LOD,      desc.minLod );
        VGL( glSamplerParameterf,  mHandle, GL_TEXTURE_LOD_BIAS,     desc.mipLodBias );
    }

    SamplerState::~SamplerState()
    {
        VGL( glDeleteSamplers, 1, &mHandle );
    }

} // namespace GFW
