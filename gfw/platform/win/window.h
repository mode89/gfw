#ifndef __GFW_PLATFORM_WIN_WINDOW_H__
#define __GFW_PLATFORM_WIN_WINDOW_H__

#include "gfw\platform\base\window.h"

namespace GFW { namespace Platform {

    class Window: public IWindow
    {
    public:
        virtual void        Tick();
        virtual void        Release();

    public:
        static IWindowRef   CreateInstance(const WindowDesc &);

        Window(const WindowDesc &);
    };

}} // namespace GFW::Platform

#endif // __GFW_PLATFORM_WIN_WINDOW_H__
