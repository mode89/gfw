#ifndef __GFW_GRAPHICS_OPENGL_FUNCTIONS_PLATFORM_H__
#define __GFW_GRAPHICS_OPENGL_FUNCTIONS_PLATFORM_H__

#include "common\platform.h"

#if PLATFORM_WIN32

#include <windows.h>
#include "opengl/glcorearb.h"
#include "opengl/wglext.h"

namespace GFW { namespace OpenGL {

    typedef PROC  (WINAPI *  PFNWGLGETPROCADDRESS)(LPCSTR);
    typedef HGLRC (WINAPI *  PFNWGLCREATECONTEXT)(HDC);
    typedef BOOL  (WINAPI *  PFNWGLMAKECURRENT)(HDC, HGLRC);
    typedef HGLRC (WINAPI *  PFNWGLGETCURRENTCONTEXT)();
    typedef BOOL  (WINAPI *  PFNWGLDELETECONTEXT)(HGLRC);
    typedef BOOL  (WINAPI *  PFNWGLCHOOSEPIXELFORMAT)(HDC, const int *, const FLOAT *, UINT, int *, UINT *);
    typedef BOOL  (WINAPI *  PFNWGLSHARELISTS)(HGLRC, HGLRC);

    extern PFNWGLGETPROCADDRESS     wglGetProcAddress;
    extern PFNWGLCREATECONTEXT      wglCreateContext;
    extern PFNWGLMAKECURRENT        wglMakeCurrent;
    extern PFNWGLGETCURRENTCONTEXT  wglGetCurrentContext;
    extern PFNWGLDELETECONTEXT      wglDeleteContext;
    extern PFNWGLCHOOSEPIXELFORMAT  wglChoosePixelFormat;
    extern PFNWGLSHARELISTS         wglShareLists;

}} // namespace GFW::OpenGL

#else
#error Unrecognized platform
#endif // Determine platform

#endif // __GFW_GRAPHICS_OPENGL_FUNCTIONS_PLATFORM_H__
