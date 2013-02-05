#include "profiler/meson.h"
#include "profiler/logger.h"

namespace Profiler {

    Meson::Meson(const Token * token)
        : mToken(token)
    {
        static Logger * logger = GetGlobalLogger();
        logger->CreateEvent(mToken, REGION_BEGIN);
    }

    Meson::~Meson()
    {
        static Logger * logger = GetGlobalLogger();
        logger->CreateEvent(mToken, REGION_END);
    }

} // namespace Profiler
