#ifndef __GFW_TESTS_RUNTIME_WINDOW_H__
#define __GFW_TESTS_RUNTIME_WINDOW_H__

#include <string>

namespace GFW {

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

    void *
    CreateDefaultWindow( const std::string & windowTitle, const WindowDesc & );

    void
    DestroyDefaultWindow( void * windowHandle );

    bool
    ProcessDefaultWindow( void * windowHandle );

} // namespace GFW

#endif // __GFW_TESTS_RUNTIME_WINDOW_H__
