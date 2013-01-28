#ifndef __GFW_GRAPHICS_OPENGL_WINDOW_H__
#define __GFW_GRAPHICS_OPENGL_WINDOW_H__

#include "gfw\platform\win\window.h"

namespace GFW { namespace OpenGL {

    class OpenglWindow: public Platform::Window
    {
    public:
        virtual void    Release();

    public:
        uint32_t        Init();

        void            SwapBuffers();

    public:
        OpenglWindow(Platform::IWindowIn);

    private:
        Platform::WindowRef     mWindowPlat;
        HDC                     mHDC;
        HGLRC                   mHRC;
    };
    AUTOREF_REFERENCE_DECLARATION(OpenglWindow);

}} // namespace GFW::OpenGL

#endif // __GFW_GRAPHICS_OPENGL_WINDOW_H__
