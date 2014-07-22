#ifndef __GFW_RUNTIME_COMMON_PASS_H__
#define __GFW_RUNTIME_COMMON_PASS_H__

#include "gfw/base/pass.h"
#include "gfw/base/shader_stage.h"
#include "gfw/runtime/common/shader_table.h"
#include "gfw/runtime/core/types_fwd.h"
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
        ConstIShaderRef         mShaders[ ShaderStage::COUNT ];
    };
    SHARED_PTR_TYPEDEFS( Pass );

} // namespace GFW

#endif // __GFW_RUNTIME_COMMON_PASS_H__
