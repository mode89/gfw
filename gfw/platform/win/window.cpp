#include "gfw\platform\win\window.h"
#include "gfw\allocator.h"

#include "common\trace.h"

namespace GFW { namespace Platform {

    using namespace Common;

    class WindowClassRegisterer
    {
    public:
        WindowClassRegisterer()
            : m_hInstance(NULL)
        {
            m_hInstance = GetModuleHandle(NULL);

            // Register window class

            WNDCLASS wc;
            ZeroMemory(&wc, sizeof(wc));
            wc.style            = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
            wc.lpfnWndProc      = (WNDPROC) Window::Proc;
            wc.hInstance        = m_hInstance;
            wc.hIcon            = LoadIcon(NULL, IDI_WINLOGO);
            wc.hCursor          = LoadCursor(NULL, IDC_ARROW);
            wc.lpszClassName    = "GFW";

            if (RegisterClass(&wc) == 0)
            {
                TRACE_MESSAGE_ERROR("Cannot register window class");
                return;
            }
        }

        ~WindowClassRegisterer()
        {
            if (UnregisterClass("GFW", m_hInstance) == 0)
            {
                TRACE_MESSAGE_ERROR("Cannot unregister window class");
            }
        }

    private:
        HINSTANCE m_hInstance;

    } g_WindowClassRegisterer;

    IWindowRef Window::CreateInstance(const WindowDesc & desc, IAllocator * a)
    {
        Window * wnd = GFW_NEW(a, Window) (desc, a);

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

    Window::Window(const WindowDesc & desc, IAllocator * a)
        : mDesc(desc)
        , mHwnd(NULL)
    {
        mAllocator = a;
    }

    Window::~Window()
    {
        int res = 0;

        res = DestroyWindow(mHwnd);
        TRACE_ASSERT(res != NULL);
    }

    uint32_t Window::Initialize()
    {
        HINSTANCE hInstance = GetModuleHandle(NULL);

        // Try to enter full screen mode

        if (mDesc.fullScreen != 0)
        {
            DEVMODE devMode;
            ZeroMemory(&devMode, sizeof(devMode));
            devMode.dmSize          = sizeof(devMode);
            devMode.dmPelsWidth     = mDesc.width;
            devMode.dmPelsHeight    = mDesc.height;
            devMode.dmBitsPerPel    = mDesc.colorBits;
            devMode.dmFields        = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

            if (ChangeDisplaySettings(&devMode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
            {
                TRACE_MESSAGE_ERROR("Cannot enter fullscreen mode");
                return 0;
            }
        }

        // Create the window

        DWORD dwExStyle = 0;
        DWORD dwStyle   = 0;

        if (mDesc.fullScreen != 0)
        {
            dwExStyle = WS_EX_APPWINDOW;
            dwStyle   = WS_POPUP;
            ShowCursor(FALSE);
        }
        else
        {
            dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
            dwStyle   = WS_OVERLAPPEDWINDOW;
        }

        RECT windowRect;
        windowRect.left   = 0;
        windowRect.top    = 0;
        windowRect.right  = mDesc.width;
        windowRect.bottom = mDesc.height;
        AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);

        mHwnd = CreateWindowEx(
            dwExStyle,
            "GFW",
            "GFW Window",
            dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
            0, 0,
            windowRect.right - windowRect.left,
            windowRect.bottom - windowRect.top,
            NULL,
            NULL,
            hInstance,
            NULL);

        if (mHwnd == NULL)
        {
            TRACE_MESSAGE_ERROR("Cannot create a window");
            return 0;
        }

        ShowWindow(mHwnd, SW_SHOW);	
        SetForegroundWindow(mHwnd);
        SetFocus(mHwnd);

        return 1;
    }

    void Window::Tick()
    {
        MSG msg;
        if (PeekMessage(&msg, mHwnd, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    LRESULT CALLBACK Window::Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        // Process all unhandled messages
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

}} // namespace GFW::Platform
