#ifndef __GFW_COMMON_TECHNIQUE_H__
#define __GFW_COMMON_TECHNIQUE_H__

#include "gfw/base/technique.h"
#include "gfw/shared/technique.h"
#include "gfw/shared/types_fwd.h"

#include <vector>

namespace GFW {

    class Technique : public ITechnique
    {
    public:
        virtual void
        Dispatch( uint32_t pass = 0 );

        virtual IShaderRef
        GetShader( ShaderStage, uint32_t pass = 0 ) const;

    public:
        Technique( TechniqueBinaryRef, const Effect * );
        ~Technique();

    private:
        typedef std::vector< IPassRef > PassVec;

        const Effect *  mEffect;
        TechniqueDesc   mDesc;
        PassVec         mPasses;
    };
    AUTOREF_REFERENCE_DECLARATION( Technique );

} // namespace GFW

#endif // __GFW_COMMON_TECHNIQUE_H__
