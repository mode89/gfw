#include "gfw\platform\win\window.h"
#include "gfw\allocator.h"

#include "trace\trace.h"

namespace GFW { namespace Platform {

    using namespace Common;

    Window::Window(const WindowDesc & desc)
        : mDesc(desc)
    {
        
    }

    uint32_t Window::Initialize()
    {
        TRACE_FAIL_MSG("Not yet implemented");
        return 0;
    }

    LRESULT CALLBACK Window::Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        // Process all unhandled messages
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    void Window::Tick()
    {
        TRACE_FAIL_MSG("Not yet implemented");
    }

    void Window::Release()
    {
        TRACE_FAIL_MSG("Not yet implemented");
    }

    IWindowRef Window::CreateInstance(const WindowDesc & desc, IAllocator * a)
    {
        Window * wnd = GFW_NEW(a, Window) (desc);

        if (wnd != NULL)
        {
            if (wnd->Initialize())
            {
                return wnd;
            }

            GFW_DELETE(a, wnd);
        }

        return NULL;
    }

}} // namespace GFW::Platform
