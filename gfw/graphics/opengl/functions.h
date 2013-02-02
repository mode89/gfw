#ifndef __GFW_GRAPHICS_OPENGL_FUNCTIONS_H__
#define __GFW_GRAPHICS_OPENGL_FUNCTIONS_H__

#include "gfw\graphics\opengl\glcorearb.h"

#if TRACE_ASSERT_ENABLED

#define TRACE_ASSERT_GL(func, ...) \
    (func != NULL) ? func(__VA_ARGS__) : NULL ; \
    { \
        TRACE_ASSERT_MESSAGE_FORMATED(func != NULL, "Function %s() is not supported by the hardware", #func); \
        \
        unsigned int errorCode = glGetError(); \
        if (errorCode != 0) \
        { \
            TRACE_ASSERT_MESSAGE_FORMATED(errorCode != 0, "Function %s() failed with code %x", #func, errorCode); \
        } \
    } \

#else
#define TRACE_ASSERT_GL(func, ...)  func(__VA_ARGS__)
#endif // TRACE_ASSERT_ENABLED

#define OPENGL_FUNCTIONS_CORE \
    F(PFNGLGETERRORPROC,            glGetError) \
    F(PFNGLGETSTRINGPROC,           glGetString) \
    F(PFNGLCLEARCOLORPROC,          glClearColor) \
    F(PFNGLCLEARPROC,               glClear) \

#define OPENGL_FUNCTIONS_EXT \
    F(PFNGLCREATESHADERPROC,        glCreateShader) \
    F(PFNGLDELETESHADERPROC,        glDeleteShader) \
    F(PFNGLSHADERSOURCEPROC,        glShaderSource) \
    F(PFNGLCOMPILESHADERPROC,       glCompileShader) \
    F(PFNGLGETSHADERIVPROC,         glGetShaderiv) \
    F(PFNGLGETSHADERINFOLOGPROC,    glGetShaderInfoLog) \

namespace GFW { namespace OpenGL {

#define F(type, func) extern type func;
    OPENGL_FUNCTIONS_CORE
    OPENGL_FUNCTIONS_EXT
#undef F

}} // namespace GFW::OpenGL

#endif // __GFW_GRAPHICS_OPENGL_FUNCTIONS_H__
