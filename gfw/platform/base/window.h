#ifndef __GFW_BASE_WINDOW_H__
#define __GFW_BASE_WINDOW_H__

#include "common\autoref.h"

namespace GFW { namespace Platform {

    struct WindowDesc
    {
        uint32_t width;
        uint32_t height;
        bool8_t  fullScreen;

        WindowDesc()
            : width(0)
            , height(0)
            , fullScreen(0)
        {}
    };

    class IWindow: public Common::AutoRefObject
    {
    public:
        virtual void    Tick() = 0;
        virtual void    Release() = 0;

    public:
        virtual ~IWindow() { }
    };
    AUTOREF_REFERENCE_DECLARATION(IWindow);

    IWindowRef CreateEmptyWindow(WindowDesc &);

}} // namespace GFW::Platform

#endif // __GFW_BASE_WINDOW_H__
