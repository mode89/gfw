#include "common/trace.h"

#include "gfw/common/window.h"

#ifdef PLATFORM_WIN32
#include <windows.h>
#endif

namespace GFW {

#ifdef PLATFORM_WIN32

    class EmptyWindow : public IWindow
    {
    public:
        EmptyWindow(uint32_t width, uint32_t height)
            : mWidth(width)
            , mHeight(height)
            , mHwnd(NULL)
        {

        }

        ~EmptyWindow()
        {
            BOOL res = DestroyWindow(mHwnd);
            TRACE_ASSERT(res != FALSE);
        }

        bool Initialize()
        {
            HINSTANCE hInstance = GetModuleHandle(NULL);

            // Try to enter full screen mode

            /*
            if (isFullscreen)
            {
                DEVMODE devMode;
                ZeroMemory(&devMode, sizeof(devMode));
                devMode.dmSize          = sizeof(devMode);
                devMode.dmPelsWidth     = mWidth;
                devMode.dmPelsHeight    = mHeight;
                devMode.dmBitsPerPel    = bitsPerPixel;
                devMode.dmFields        = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

                if (ChangeDisplaySettings(&devMode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
                {
                    TRACE_ERROR("Cannot enter fullscreen mode");
                    return 0;
                }
            }
            */

            // Create the window

            DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
            DWORD dwStyle   = WS_OVERLAPPEDWINDOW;

            RECT windowRect;
            windowRect.left   = 0;
            windowRect.top    = 0;
            windowRect.right  = mWidth;
            windowRect.bottom = mHeight;
            AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);

            mHwnd = CreateWindowEx(
                dwExStyle,
                "GFW::EmptyWindow",
                "GFW",
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
                TRACE_ERROR("Cannot create a window");
                return false;
            }

            ShowWindow(mHwnd, SW_SHOW);	
            SetForegroundWindow(mHwnd);
            SetFocus(mHwnd);

            return true;
        }

        void EmptyWindow::Tick()
        {
            MSG msg;
            if (PeekMessage(&msg, mHwnd, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        WindowHandle GetHandle()
        {
            return mHwnd;
        }

        static LRESULT CALLBACK Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
        {
            // Process all unhandled messages
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
        }

    private:
        uint32_t    mWidth;
        uint32_t    mHeight;
        HWND        mHwnd;
    };

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
            wc.lpfnWndProc      = (WNDPROC) EmptyWindow::Proc;
            wc.hInstance        = m_hInstance;
            wc.hIcon            = LoadIcon(NULL, IDI_WINLOGO);
            wc.hCursor          = LoadCursor(NULL, IDC_ARROW);
            wc.lpszClassName    = "GFW::EmptyWindow";

            if (RegisterClass(&wc) == 0)
            {
                TRACE_ERROR("Cannot register window class");
                return;
            }
        }

        ~WindowClassRegisterer()
        {
            if (UnregisterClass("GFW::EmptyWindow", m_hInstance) == 0)
            {
                TRACE_ERROR("Cannot unregister window class");
            }
        }

    private:
        HINSTANCE m_hInstance;

    } g_WindowClassRegisterer;

#else
#error Unsupported platform
#endif // Recognize a platform

    IWindowRef CreateEmptyWindow(uint32_t width, uint32_t height)
    {
        EmptyWindow * window = new EmptyWindow(width, height);

        if (window == NULL)
        {
            TRACE_ERROR("Cannot allocate memory!");
            return NULL;
        }

        if (!window->Initialize())
        {
            TRACE_ERROR("Cannot initialize the window!");
            delete window;
            return NULL;
        }

        return window;
    }

} // namespace Common
