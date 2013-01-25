#ifndef __GFW_PLATFORM_WIN_WINDOW_H__
#define __GFW_PLATFORM_WIN_WINDOW_H__

#include "gfw\platform\base\window.h"
#include "common\allocator.h"

#include <windows.h>

namespace GFW { namespace Platform {

    class Window: public IWindow
    {
    public:
        virtual void                Tick();
        virtual void                Release();

    public:
        static IWindowRef           CreateInstance(const WindowDesc &, Common::IAllocator * a);
        uint32_t                    Initialize();

    private:
        static LRESULT CALLBACK     Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    public:
        Window(const WindowDesc &);

    private:
        WindowDesc                  mDesc;
    };

}} // namespace GFW::Platform

#endif // __GFW_PLATFORM_WIN_WINDOW_H__

