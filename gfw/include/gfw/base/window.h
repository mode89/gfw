#ifndef __GFW_BASE_WINDOW_H__
#define __GFW_BASE_WINDOW_H__

namespace GFW {

    typedef void * WindowHandle; // Platform specific window handle

    struct WindowDesc
    {
        uint32_t    width;
        uint32_t    height;
        bool        isFullscreen;

        WindowDesc()
            : width(0)
            , height(0)
            , isFullscreen(false)
        {}
    };

    WindowHandle    CreateDefaultWindow(const WindowDesc &);
    void            DestroyDefaultWindow(const WindowHandle);
    bool            ProcessDefaultWindow(const WindowHandle);

} // namespace GFW

#endif // __GFW_BASE_WINDOW_H__
