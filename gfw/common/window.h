#ifndef __GFW_COMMON_WINDOW_H__
#define __GFW_COMMON_WINDOW_H__

#include "common/autoref.h"

namespace GFW {

    typedef void * WindowHandle; // Platform-specific handle of the window

    class IWindow : public Common::ARefCounted
    {
    public:
        virtual void            Tick() = 0;

        virtual WindowHandle    GetHandle() = 0;

    public:
        virtual ~IWindow() {}
    };
    AUTOREF_REFERENCE_DECLARATION(IWindow);

    IWindowRef CreateEmptyWindow(uint32_t width, uint32_t height);

} // namespace GFW

#endif // __GFW_COMMON_WINDOW_H__
