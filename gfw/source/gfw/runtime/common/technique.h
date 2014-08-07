#ifndef __GFW_RUNTIME_COMMON_TECHNIQUE_H__
#define __GFW_RUNTIME_COMMON_TECHNIQUE_H__

#include "gfw/technique.h"
#include "gfw/runtime/common/shader_table.h"
#include "gfw/runtime/common/types_fwd.h"
#include "gfw/shared/types_fwd.h"

#include <vector>

namespace GFW {

    class Technique : public ITechnique
    {
    public:
        virtual const TechniqueDesc &
        GetDesc() const { return mDesc; }

        virtual void
        Dispatch( uint32_t pass ) const;

        virtual ConstIShaderRef
        GetShader( ShaderStage, uint32_t pass ) const;

    public:
        Technique( const TechniqueBinary &, ShaderTable &, IDeviceIn );
        ~Technique();

    private:
        typedef std::vector< PassRef > PassVec;

        TechniqueDesc   mDesc;
        PassVec         mPasses;
    };
    SHARED_PTR_TYPEDEFS( Technique );

} // namespace GFW

#endif // __GFW_RUNTIME_COMMON_TECHNIQUE_H__
