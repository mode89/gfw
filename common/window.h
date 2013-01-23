#ifndef __COMMON_WINDOW_H__
#define __COMMON_WINDOW_H__

#include "callback.h"

namespace Common {

    class IWindow
    {
    public:
        // Show the window
		virtual void    Show() = 0;

		// Set redraw callback
		virtual void    SetRedrawCallback(ICallback *) = 0;

    public:
        virtual         ~IWindow() {}
    };

} // namespace Common

#endif // __COMMON_WINDOW_H__
