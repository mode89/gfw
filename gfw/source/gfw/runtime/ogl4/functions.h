#ifndef __GFW_RUNTIME_CORE_FUNCTIONS_H__
#define __GFW_RUNTIME_CORE_FUNCTIONS_H__

#include "cmn/platform.h"

#include "opengl/glcorearb.h"

#if CMN_DEBUG
    #define VGL( func, ... ) \
        ( func != NULL ) ? func( __VA_ARGS__ ) : ( throw -1 ) ; \
        { \
            CMN_ASSERT_MSG( func != NULL, "Function %s() is not supported by the hardware", #func ); \
            \
            unsigned int errorCode = glGetError(); \
            if ( errorCode != 0 ) \
            { \
                CMN_ASSERT_MSG( errorCode != 0, "Function %s() failed with code %x", #func, errorCode ); \
            } \
        }
#else
    #define VGL( func, ...)  func(__VA_ARGS__ )
#endif // CMN_DEBUG

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
    F(PFNGLENABLEPROC,                      glEnable) \
    F(PFNGLDISABLEPROC,                     glDisable) \
    \
    F(PFNGLTEXIMAGE2DPROC,                  glTexImage2D) \
    F(PFNGLTEXPARAMETERIPROC,               glTexParameteri) \
    \
    F(PFNGLDRAWBUFFERPROC,                  glDrawBuffer) \
    F(PFNGLREADBUFFERPROC,                  glReadBuffer) \

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
    F(PFNGLPROGRAMPARAMETERIPROC,           glProgramParameteri) \
    F(PFNGLGETPROGRAMINTERFACEIVPROC,       glGetProgramInterfaceiv) \
    F(PFNGLGETPROGRAMRESOURCENAMEPROC,      glGetProgramResourceName) \
    F(PFNGLGETPROGRAMRESOURCELOCATIONPROC,  glGetProgramResourceLocation) \
    F(PFNGLGETPROGRAMRESOURCEIVPROC,        glGetProgramResourceiv) \
    \
    F(PFNGLGENPROGRAMPIPELINESPROC,         glGenProgramPipelines) \
    F(PFNGLDELETEPROGRAMPIPELINESPROC,      glDeleteProgramPipelines) \
    F(PFNGLBINDPROGRAMPIPELINEPROC,         glBindProgramPipeline) \
    F(PFNGLUSEPROGRAMSTAGESPROC,            glUseProgramStages) \
    \
    F(PFNGLGETATTRIBLOCATIONPROC,           glGetAttribLocation) \
    \
    F(PFNGLGENBUFFERSPROC,                  glGenBuffers) \
    F(PFNGLDELETEBUFFERSPROC,               glDeleteBuffers) \
    F(PFNGLBINDBUFFERPROC,                  glBindBuffer) \
    F(PFNGLBUFFERDATAPROC,                  glBufferData) \
    F(PFNGLBUFFERSUBDATAPROC,               glBufferSubData) \
    F(PFNGLMAPBUFFERPROC,                   glMapBuffer) \
    F(PFNGLMAPBUFFERRANGEPROC,              glMapBufferRange) \
    F(PFNGLUNMAPBUFFERPROC,                 glUnmapBuffer) \
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
    \
    F(PFNGLDEPTHFUNCPROC,                   glDepthFunc) \
    F(PFNGLDEPTHMASKPROC,                   glDepthMask) \
    F(PFNGLSTENCILOPPROC,                   glStencilOp) \
    F(PFNGLSTENCILOPSEPARATEPROC,           glStencilOpSeparate) \
    F(PFNGLSTENCILFUNCPROC,                 glStencilFunc) \
    F(PFNGLSTENCILFUNCSEPARATEPROC,         glStencilFuncSeparate) \
    F(PFNGLSTENCILMASKPROC,                 glStencilMask) \
    F(PFNGLSTENCILMASKSEPARATEPROC,         glStencilMaskSeparate) \
    \
    F(PFNGLPOLYGONMODEPROC,                 glPolygonMode) \
    F(PFNGLPOLYGONOFFSETPROC,               glPolygonOffset) \
    F(PFNGLCULLFACEPROC,                    glCullFace) \
    F(PFNGLFRONTFACEPROC,                   glFrontFace) \
    \
    F(PFNGLGENSAMPLERSPROC,                 glGenSamplers) \
    F(PFNGLDELETESAMPLERSPROC,              glDeleteSamplers) \
    F(PFNGLSAMPLERPARAMETERIPROC,           glSamplerParameteri) \
    F(PFNGLSAMPLERPARAMETERFVPROC,          glSamplerParameterfv) \
    F(PFNGLSAMPLERPARAMETERFPROC,           glSamplerParameterf) \
    \
    F(PFNGLGENTEXTURESPROC,                 glGenTextures) \
    F(PFNGLDELETETEXTURESPROC,              glDeleteTextures) \
    F(PFNGLACTIVETEXTUREPROC,               glActiveTexture) \
    F(PFNGLBINDTEXTUREPROC,                 glBindTexture) \
    \
    F(PFNGLGENFRAMEBUFFERSPROC,             glGenFramebuffers) \
    F(PFNGLDELETEFRAMEBUFFERSPROC,          glDeleteFramebuffers) \
    F(PFNGLBINDFRAMEBUFFERPROC,             glBindFramebuffer) \
    F(PFNGLFRAMEBUFFERTEXTUREPROC,          glFramebufferTexture) \
    F(PFNGLBLITFRAMEBUFFERPROC,             glBlitFramebuffer) \
    F(PFNGLDRAWBUFFERSPROC,                 glDrawBuffers) \
    F(PFNGLCHECKFRAMEBUFFERSTATUSPROC,      glCheckFramebufferStatus) \

#ifdef CMN_WIN32

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

#endif // __GFW_RUNTIME_CORE_FUNCTIONS_H__
