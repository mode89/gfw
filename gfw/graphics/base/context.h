#ifndef __GFW_GRAPHICS_BASE_CONTEXT_H__
#define __GFW_GRAPHICS_BASE_CONTEXT_H__

#include "common\autoref.h"
#include "gfw\graphics\base\clear_params.h"

namespace GFW {

    class IContext: public Common::AutoRefObject
    {
    public:
        virtual void    Clear(ClearParams &) = 0;
        virtual void    Present() = 0;
        virtual void    Release() = 0;

    public:
        virtual         ~IContext() { }
    };
    AUTOREF_REFERENCE_DECLARATION(IContext);

} // namespace GFW

#endif // __GFW_GRAPHICS_BASE_CONTEXT_H__
