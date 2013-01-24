#include "gfw\platform\win\window.h"
#include "gfw\allocator.h"

#include "trace\trace.h"

namespace GFW { namespace Platform {

    using namespace Common;

    Window::Window(const WindowDesc &)
    {
        TRACE_FAIL_MSG("Not yet implemented");
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
        return GFW_NEW(a, Window) (desc);
    }

}} // namespace GFW::Platform
