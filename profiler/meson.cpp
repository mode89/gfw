#include "profiler/meson.h"
#include "profiler/manager.h"

namespace Profiler {

    Meson::Meson(const Token * token)
        : mToken(token)
    {
        static Manager * manager = GetGlobalManager();
        manager->CreateEvent(mToken, REGION_BEGIN);
    }

    Meson::~Meson()
    {
        static Manager * manager = GetGlobalManager();
        manager->CreateEvent(mToken, REGION_END);
    }

} // namespace Profiler
