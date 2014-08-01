#ifndef __GFW_RUNTIME_COMMON_PASS_H__
#define __GFW_RUNTIME_COMMON_PASS_H__

#include "gfw/pass.h"
#include "gfw/shader_stage.h"
#include "gfw/runtime/common/shader_table.h"
#include "gfw/runtime/ogl4/types_fwd.h"
#include "gfw/shared/types_fwd.h"

namespace GFW {

    class Pass : public IPass
    {
    public:
        virtual void
        Dispatch() const;

        virtual ConstIShaderRef
        GetShader( ShaderStage stage ) const { return mShaders[ stage ]; }

    public:
        Pass( const PassBinary &, const ShaderTable &, IDeviceIn );
        ~Pass();

    private:
        std::weak_ptr<IDevice>  mDevice;
        ConstIShaderRef         mShaders[ SHADER_STAGE_COUNT ];
    };
    SHARED_PTR_TYPEDEFS( Pass );

} // namespace GFW

#endif // __GFW_RUNTIME_COMMON_PASS_H__
