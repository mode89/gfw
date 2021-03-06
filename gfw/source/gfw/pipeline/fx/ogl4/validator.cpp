#include "cmn/trace.h"
#include "gfw/shader_stage.h"
#include "gfw/pipeline/fx/ogl4/shader_builder_exception.h"
#include "gfw/pipeline/fx/ogl4/validator.h"
#include "opengl/glcorearb.h"

#include "opengl/wglext.h"
#include <windows.h>

#define VGL( func, ... )   \
    func( __VA_ARGS__ );        \
    if ( glGetError() )         \
    {                           \
        CMN_FAIL();           \
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
            CMN_ASSERT( sLibrary != NULL );

            // Load Windows specific functions

CMN_WARNING_PUSH
CMN_WARNING_DISABLE_MSVC( 4191 ) // Unsafe conversion from FARPROC

            wglGetProcAddress = reinterpret_cast<PFNWGLGETPROCADDRESS>( GetProcAddress( sLibrary, "wglGetProcAddress" ) );
            CMN_ASSERT( wglGetProcAddress != NULL );

            wglCreateContext = reinterpret_cast<PFNWGLCREATECONTEXT>( GetProcAddress( sLibrary, "wglCreateContext" ) );
            CMN_ASSERT( wglCreateContext != NULL );

            wglMakeCurrent = reinterpret_cast<PFNWGLMAKECURRENT>( GetProcAddress( sLibrary, "wglMakeCurrent" ) );
            CMN_ASSERT( wglMakeCurrent != NULL );

            wglDeleteContext = reinterpret_cast<PFNWGLDELETECONTEXT>( GetProcAddress( sLibrary, "wglDeleteContext" ) );
            CMN_ASSERT( wglDeleteContext != NULL );

            // Create empty window

            sHwnd = CreateWindow("STATIC", "Window",
                WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
                0, 0, 16, 16, NULL, NULL, GetModuleHandle( NULL ), NULL);
            CMN_ASSERT( sHwnd != NULL );

            sHdc = GetDC( sHwnd );
            CMN_ASSERT( sHdc != NULL );

            // Create temp rendering context

            PIXELFORMATDESCRIPTOR pfd;
            ZeroMemory( &pfd, sizeof( pfd ) );

            int pixelFormat = ChoosePixelFormat( sHdc, &pfd );
            CMN_ASSERT( pixelFormat != 0 );

            int res = SetPixelFormat( sHdc, pixelFormat, &pfd );
            CMN_ASSERT( res != FALSE );

            sHrc = wglCreateContext( sHdc );
            CMN_ASSERT( sHrc != NULL );

            res = wglMakeCurrent( sHdc, sHrc );
            CMN_ASSERT( res != FALSE );

            // Load core functions

#define F( type, func )   func = reinterpret_cast<type>( GetProcAddress( sLibrary, #func ) );
            OPENGL_FUNCTIONS_CORE
#undef F

            // Load extended functions

#define F( type, func )   func = reinterpret_cast<type>( wglGetProcAddress( #func ) );
            OPENGL_FUNCTIONS_EXT
#undef F

CMN_WARNING_POP

            // Release resources

            res = wglMakeCurrent( sHdc, 0 );
            CMN_ASSERT( res != 0 );
        }
    }

    void ReleaseValidator()
    {
        if ( --sValidatorRefCount == 0 )
        {
            BOOL res;

            res = wglDeleteContext( sHrc );
            CMN_ASSERT( res != 0 );

            res = ReleaseDC( sHwnd, sHdc );
            CMN_ASSERT( res != 0 );

            res = DestroyWindow( sHwnd );
            CMN_ASSERT( res != 0 );
        }
    }

    void Validate( ShaderStage stage, const char * source )
    {
        CMN_ASSERT( source != NULL );

        int32_t compileStatus = GL_FALSE;

        wglMakeCurrent( sHdc, sHrc );
        {
            uint32_t shaderType = 0;
            switch ( stage )
            {
            case SHADER_STAGE_VERTEX:
                shaderType = GL_VERTEX_SHADER;
                break;
            case SHADER_STAGE_PIXEL:
                shaderType = GL_FRAGMENT_SHADER;
                break;
            default:
                CMN_FAIL();
            }

            uint32_t shader = VGL( glCreateShader, shaderType );
            VGL( glShaderSource, shader, 1, &source, NULL );
            VGL( glCompileShader, shader );

            VGL( glGetShaderiv, shader, GL_COMPILE_STATUS, &compileStatus );

            if ( compileStatus == GL_FALSE )
            {
                char infoLog[1024] = { 0 };
                VGL( glGetShaderInfoLog, shader, sizeof( infoLog ), NULL, infoLog );
                CMN_MSG( "Shader compilation log:\n%s", infoLog );
            }

            VGL( glDeleteShader, shader );
        }
        wglMakeCurrent( sHdc, NULL );

        CMN_THROW_IF( compileStatus == GL_FALSE,
            ShaderBuilderException::ValidationError() );
    }

} // namespace GFW
