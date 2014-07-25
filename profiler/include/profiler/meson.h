#ifndef __PROFILER_MESON_H__
#define __PROFILER_MESON_H__

#include "cmn/typedefs.h"
#include "profiler/token.h"

namespace Profiler {

    class Meson
    {
    public:
        Meson(const Token *);
        ~Meson();

    private:
        const Token *   mToken;
    };

} // namespace Profiler

#endif // __PROFILER_MESON_H__
