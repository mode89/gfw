#include "cmn/trace.h"
#include "gfw/base/shader_stage.h"
#include "gfw/pipeline/shader_builder_exception.h"
#include "gfw/pipeline/validator.h"
#include "opengl/glcorearb.h"

#include "opengl/wglext.h"
#include <windows.h>

#define TRACE_GL( func, ... )   \
    func( __VA_ARGS__ );        \
    if ( glGetError() )         \
    {                           \
        TRACE_FAIL();           \
    }                           \

namespace GFW {

#define OPENGL_FUNCTIONS_CORE \
    F( PFNGLGETERRORPROC,           glGetError )            \

#define OPENGL_FUNCTIONS_EXT \
    F( PFNGLCREATESHADERPROC,       glCreateShader )        \
    F( PFNGLDELETESHADERPROC,       glDeleteShader )        \
    F( PFNGLSHADERSOURCEPROC,       glShaderSource )        \
    F( PFNGLCOMPILESHADERPROC,      glCompileShader )       \
    F( PFNGLGETSHADERIVPROC,        glGetShaderiv )         \
    F( PFNGLGETSHADERINFOLOGPROC,   glGetShaderInfoLog )    \

    typedef PROC  (WINAPI *  PFNWGLGETPROCADDRESS)(LPCSTR);
    typedef HGLRC (WINAPI *  PFNWGLCREATECONTEXT)(HDC);
    typedef BOOL  (WINAPI *  PFNWGLDELETECONTEXT)(HGLRC);
    typedef BOOL  (WINAPI *  PFNWGLMAKECURRENT)(HDC, HGLRC);
    typedef HGLRC (WINAPI *  PFNWGLGETCURRENTCONTEXT)();
    typedef BOOL  (WINAPI *  PFNWGLSHARELISTS)(HGLRC, HGLRC);
    typedef BOOL  (WINAPI *  PFNWGLCHOOSEPIXELFORMAT) (HDC, const int *, const FLOAT *, UINT, int *, UINT *);

    static uint32_t sValidatorRefCount  = 0;
    static HMODULE  sLibrary            = NULL;
    static HWND     sHwnd               = NULL;
    static HDC      sHdc                = NULL;
    static HGLRC    sHrc                = NULL;

    static PFNWGLGETPROCADDRESS wglGetProcAddress   = NULL;
    static PFNWGLCREATECONTEXT  wglCreateContext    = NULL;
    static PFNWGLDELETECONTEXT  wglDeleteContext    = NULL;
    static PFNWGLMAKECURRENT    wglMakeCurrent      = NULL;

#define F( type, func ) static type func = NULL;
    OPENGL_FUNCTIONS_CORE
    OPENGL_FUNCTIONS_EXT
#undef F

    void AcquireValidator()
    {
        if ( sValidatorRefCount++ == 0 )
        {
            sLibrary = LoadLibrary("opengl32.dll");
            TRACE_ASSERT( sLibrary != NULL );

            // Load Windows specific functions

PLAT_WARNING_PUSH
PLAT_WARNING_DISABLE_MSVC( 4191 ) // Unsafe conversion from FARPROC

            wglGetProcAddress = reinterpret_cast<PFNWGLGETPROCADDRESS>( GetProcAddress( sLibrary, "wglGetProcAddress" ) );
            TRACE_ASSERT( wglGetProcAddress != NULL );

            wglCreateContext = reinterpret_cast<PFNWGLCREATECONTEXT>( GetProcAddress( sLibrary, "wglCreateContext" ) );
            TRACE_ASSERT( wglCreateContext != NULL );

            wglMakeCurrent = reinterpret_cast<PFNWGLMAKECURRENT>( GetProcAddress( sLibrary, "wglMakeCurrent" ) );
            TRACE_ASSERT( wglMakeCurrent != NULL );

            wglDeleteContext = reinterpret_cast<PFNWGLDELETECONTEXT>( GetProcAddress( sLibrary, "wglDeleteContext" ) );
            TRACE_ASSERT( wglDeleteContext != NULL );

            // Create empty window

            sHwnd = CreateWindow("STATIC", "Window",
                WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
                0, 0, 16, 16, NULL, NULL, GetModuleHandle( NULL ), NULL);
            TRACE_ASSERT( sHwnd != NULL );

            sHdc = GetDC( sHwnd );
            TRACE_ASSERT( sHdc != NULL );

            // Create temp rendering context

            PIXELFORMATDESCRIPTOR pfd;
            ZeroMemory( &pfd, sizeof( pfd ) );

            int pixelFormat = ChoosePixelFormat( sHdc, &pfd );
            TRACE_ASSERT( pixelFormat != 0 );

            int res = SetPixelFormat( sHdc, pixelFormat, &pfd );
            TRACE_ASSERT( res != FALSE );

            sHrc = wglCreateContext( sHdc );
            TRACE_ASSERT( sHrc != NULL );

            res = wglMakeCurrent( sHdc, sHrc );
            TRACE_ASSERT( res != FALSE );

            // Load core functions

#define F( type, func )   func = reinterpret_cast<type>( GetProcAddress( sLibrary, #func ) );
            OPENGL_FUNCTIONS_CORE
#undef F

            // Load extended functions

#define F( type, func )   func = reinterpret_cast<type>( wglGetProcAddress( #func ) );
            OPENGL_FUNCTIONS_EXT
#undef F

PLAT_WARNING_POP

            // Release resources

            res = wglMakeCurrent( sHdc, 0 );
            TRACE_ASSERT( res != 0 );
        }
    }

    void ReleaseValidator()
    {
        if ( --sValidatorRefCount == 0 )
        {
            BOOL res;

            res = wglDeleteContext( sHrc );
            TRACE_ASSERT( res != 0 );

            res = ReleaseDC( sHwnd, sHdc );
            TRACE_ASSERT( res != 0 );

            res = DestroyWindow( sHwnd );
            TRACE_ASSERT( res != 0 );
        }
    }

    void Validate( ShaderStage stage, const char * source )
    {
        TRACE_ASSERT( source != NULL );

        int32_t compileStatus = GL_FALSE;

        wglMakeCurrent( sHdc, sHrc );
        {
            uint32_t shaderType = 0;
            switch ( stage )
            {
            case ShaderStage::VERTEX:
                shaderType = GL_VERTEX_SHADER;
                break;
            case ShaderStage::PIXEL:
                shaderType = GL_FRAGMENT_SHADER;
                break;
            default:
                TRACE_FAIL();
            }

            uint32_t shader = TRACE_GL( glCreateShader, shaderType );
            TRACE_GL( glShaderSource, shader, 1, &source, NULL );
            TRACE_GL( glCompileShader, shader );

            TRACE_GL( glGetShaderiv, shader, GL_COMPILE_STATUS, &compileStatus );

            if ( compileStatus == GL_FALSE )
            {
                char infoLog[1024] = { 0 };
                TRACE_GL( glGetShaderInfoLog, shader, sizeof( infoLog ), NULL, infoLog );
                TRACE_MSG( "Shader compilation log:\n%s", infoLog );
            }

            TRACE_GL( glDeleteShader, shader );
        }
        wglMakeCurrent( sHdc, NULL );

        TRACE_THROW_IF( compileStatus == GL_FALSE,
            ShaderBuilderException::ValidationError() );
    }

} // namespace GFW
