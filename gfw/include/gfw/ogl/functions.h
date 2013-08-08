#ifndef __GFW_OGL_FUNCTIONS_H__
#define __GFW_OGL_FUNCTIONS_H__

#include "common/platform.h"

#include "opengl/glcorearb.h"

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
        }
#else
    #define TRACE_ASSERT_GL(func, ...)  func(__VA_ARGS__)
#endif // TRACE_ASSERT_ENABLED

#define OPENGL_FUNCTIONS_CORE \
    F(PFNGLGETERRORPROC,                    glGetError) \
    F(PFNGLGETSTRINGPROC,                   glGetString) \
    F(PFNGLCLEARCOLORPROC,                  glClearColor) \
    F(PFNGLCLEARDEPTHPROC,                  glClearDepth) \
    F(PFNGLCLEARPROC,                       glClear) \
    F(PFNGLDRAWARRAYSPROC,                  glDrawArrays) \
    F(PFNGLDRAWELEMENTSPROC,                glDrawElements) \
    F(PFNGLFLUSHPROC,                       glFlush) \
    F(PFNGLFINISHPROC,                      glFinish) \

#define OPENGL_FUNCTIONS_EXT \
    F(PFNGLCREATESHADERPROC,                glCreateShader) \
    F(PFNGLDELETESHADERPROC,                glDeleteShader) \
    F(PFNGLSHADERSOURCEPROC,                glShaderSource) \
    F(PFNGLCOMPILESHADERPROC,               glCompileShader) \
    F(PFNGLGETSHADERIVPROC,                 glGetShaderiv) \
    F(PFNGLGETSHADERINFOLOGPROC,            glGetShaderInfoLog) \
    \
    F(PFNGLCREATEPROGRAMPROC,               glCreateProgram) \
    F(PFNGLDELETEPROGRAMPROC,               glDeleteProgram) \
    F(PFNGLATTACHSHADERPROC,                glAttachShader) \
    F(PFNGLDETACHSHADERPROC,                glDetachShader) \
    F(PFNGLLINKPROGRAMPROC,                 glLinkProgram) \
    F(PFNGLUSEPROGRAMPROC,                  glUseProgram) \
    F(PFNGLGETPROGRAMIVPROC,                glGetProgramiv) \
    F(PFNGLGETPROGRAMINFOLOGPROC,           glGetProgramInfoLog) \
    \
    F(PFNGLGETATTRIBLOCATIONPROC,           glGetAttribLocation) \
    \
    F(PFNGLGENBUFFERSPROC,                  glGenBuffers) \
    F(PFNGLDELETEBUFFERSPROC,               glDeleteBuffers) \
    F(PFNGLBINDBUFFERPROC,                  glBindBuffer) \
    F(PFNGLBUFFERDATAPROC,                  glBufferData) \
    F(PFNGLBUFFERSUBDATAPROC,               glBufferSubData) \
    \
    F(PFNGLVERTEXATTRIBPOINTERPROC,         glVertexAttribPointer) \
    F(PFNGLENABLEVERTEXATTRIBARRAYPROC,     glEnableVertexAttribArray) \
    F(PFNGLDISABLEVERTEXATTRIBARRAYPROC,    glDisableVertexAttribArray) \
    \
    F(PFNGLENABLEIPROC,                     glEnablei) \
    F(PFNGLDISABLEIPROC,                    glDisablei) \
    \
    F(PFNGLBLENDFUNCSEPARATEIPROC,          glBlendFuncSeparatei) \
    F(PFNGLBLENDEQUATIONSEPARATEIPROC,      glBlendEquationSeparatei) \
    F(PFNGLCOLORMASKIPROC,                  glColorMaski) \
    F(PFNGLBLENDCOLORPROC,                  glBlendColor) \

#ifdef PLATFORM_WIN32

    typedef PROC  (WINAPI *  PFNWGLGETPROCADDRESS)(LPCSTR);
    typedef HGLRC (WINAPI *  PFNWGLCREATECONTEXT)(HDC);
    typedef BOOL  (WINAPI *  PFNWGLDELETECONTEXT)(HGLRC);
    typedef BOOL  (WINAPI *  PFNWGLMAKECURRENT)(HDC, HGLRC);
    typedef HGLRC (WINAPI *  PFNWGLGETCURRENTCONTEXT)();
    typedef BOOL  (WINAPI *  PFNWGLSHARELISTS)(HGLRC, HGLRC);
    typedef BOOL  (WINAPI *  PFNWGLCHOOSEPIXELFORMAT) (HDC, const int *, const FLOAT *, UINT, int *, UINT *);

    #define OPENGL_FUNCTIONS_PLAT \
        F(PFNWGLGETPROCADDRESS,                 wglGetProcAddress) \
        F(PFNWGLCREATECONTEXT,                  wglCreateContext) \
        F(PFNWGLDELETECONTEXT,                  wglDeleteContext) \
        F(PFNWGLMAKECURRENT,                    wglMakeCurrent) \
        F(PFNWGLGETCURRENTCONTEXT,              wglGetCurrentContext) \
        F(PFNWGLSHARELISTS,                     wglShareLists) \
        F(PFNWGLCHOOSEPIXELFORMAT,              wglChoosePixelFormat)

#endif // PLATFORM_WIN32

namespace GFW {

#define F(type, func) extern type func;
    OPENGL_FUNCTIONS_CORE
    OPENGL_FUNCTIONS_EXT
    OPENGL_FUNCTIONS_PLAT
#undef F

} // namespace GFW

#endif // __GFW_OGL_FUNCTIONS_H__
