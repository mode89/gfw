#ifndef __GFW_GRAPHICS_OPENGL_WINDOW_H__
#define __GFW_GRAPHICS_OPENGL_WINDOW_H__

#include "gfw\platform\win\window.h"

namespace GFW { namespace OpenGL {

    class OpenglWindow: public Platform::IWindow
    {
    public:
        virtual void    Tick();

    public:
        uint32_t        Init();

        void            SwapBuffers();

    public:
        OpenglWindow(Platform::IWindowIn, Common::IAllocator *);
        ~OpenglWindow();

    private:
        Platform::WindowRef     mWindowPlat;
        HWND                    mHWND;
        HDC                     mHDC;
        HGLRC                   mHRC;
    };
    AUTOREF_REFERENCE_DECLARATION(OpenglWindow);

}} // namespace GFW::OpenGL

#endif // __GFW_GRAPHICS_OPENGL_WINDOW_H__
