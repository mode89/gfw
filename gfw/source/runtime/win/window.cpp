#include "common/trace.h"

#include "gfw/base/window.h"

#include <windows.h>

namespace GFW {

    LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, LPARAM lParam, WPARAM wParam)
    {
        return DefWindowProc(hWnd, uMsg, lParam, wParam);
    }

    // Singleton with reference counting

    class WindowClass
    {
    public:
        bool Initialize()
        {
            HINSTANCE hInst = GetModuleHandle(NULL);

            WNDCLASS wc;
            ZeroMemory(&wc, sizeof(wc));
            wc.style            = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
            wc.lpfnWndProc      = (WNDPROC) WindowProc;
            wc.hInstance        = hInst;
            wc.hIcon            = LoadIcon(NULL, IDI_WINLOGO);
            wc.hCursor          = LoadCursor(NULL, IDC_ARROW);
            wc.lpszClassName    = kName;

            if (RegisterClass(&wc) == 0)
            {
                TRACE_ERR("Failed to register window class");
                return false;
            }

            return true;
        }

        ~WindowClass()
        {
            HINSTANCE hInst = GetModuleHandle(NULL);
            if (UnregisterClass(kName, hInst) == 0)
            {
                TRACE_ERR("Failed to unregister the window class");
                return;
            }
        }

        const char * GetName()
        {
            return kName;
        }

        static WindowClass * GetInstance()
        {
            if (mInstance == NULL)
            {
                mInstance = new WindowClass;

                if (!mInstance->Initialize())
                {
                    TRACE_ERR("Failed to initialize the window class");
                    delete mInstance;
                    mInstance = NULL;
                    return NULL;
                }
            }

            mRefCounter ++;
            return mInstance;
        }

        static void Release()
        {
            TRACE_ASSERT(mInstance != NULL);

            mRefCounter --;
            if (mRefCounter == 0)
            {
                delete mInstance;
                mInstance = 0;
            }
        }

    private:
        static WindowClass *    mInstance;
        static uint32_t         mRefCounter;

        static const char *     kName;
    };

    WindowClass *   WindowClass::mInstance   = NULL;
    uint32_t        WindowClass::mRefCounter = 0;
    const char *    WindowClass::kName       = "gfw_default_windows";

    WindowHandle CreateDefaultWindow(const WindowDesc & desc)
    {
        HINSTANCE hInst = GetModuleHandle(NULL);

        // Acquire the window class

        WindowClass * wndClass = WindowClass::GetInstance();

        // Try to enter full screen mode

        if (desc.isFullscreen)
        {
            DEVMODE devMode;
            ZeroMemory(&devMode, sizeof(devMode));
            devMode.dmSize          = sizeof(devMode);
            devMode.dmPelsWidth     = desc.width;
            devMode.dmPelsHeight    = desc.height;
            devMode.dmBitsPerPel    = 32;
            devMode.dmFields        = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

            if (ChangeDisplaySettings(&devMode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
            {
                TRACE_ERR("Failed to enter fullscreen mode");
                return NULL;
            }
        }

        // Create the window

        DWORD dwExStyle = 0;
        DWORD dwStyle   = 0;

        if (desc.isFullscreen)
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
        windowRect.right  = desc.width;
        windowRect.bottom = desc.height;
        AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);

        HWND handle = CreateWindowEx(
            dwExStyle,
            wndClass->GetName(),
            "Default Window",
            dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
            0, 0,
            windowRect.right - windowRect.left,
            windowRect.bottom - windowRect.top,
            NULL,
            NULL,
            hInst,
            NULL);

        if (handle == NULL)
        {
            TRACE_ERR("Failed to create a window");
            return NULL;
        }

        ShowWindow(handle, SW_SHOW);
        SetForegroundWindow(handle);
        SetFocus(handle);

        return handle;
    }

    void DestroyDefaultWindow(const WindowHandle handle)
    {
        TRACE_ASSERT(handle != NULL);
        if (handle != NULL)
        {
            int res = 0;

            res = DestroyWindow(static_cast<HWND>(handle));
            TRACE_ASSERT( res != 0 );

            WindowClass::Release();
        }
    }

    bool ProcessDefaultWindow(const WindowHandle handle)
    {
        MSG msg;

        if (PeekMessage(&msg, static_cast<HWND>(handle), 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_SYSCOMMAND && msg.wParam == SC_CLOSE)
            {
                return false;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        return true;
    }

} // namespace GFW
