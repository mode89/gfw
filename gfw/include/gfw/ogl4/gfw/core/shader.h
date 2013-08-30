#ifndef __GFW_CORE_SHADER_H__
#define __GFW_CORE_SHADER_H__

#include "gfw/base/shader.h"
#include "gfw/common/device_child.h"
#include "gfw/core/types_fwd.h"

namespace GFW {

    class Shader: public ADeviceChild<IShader>
    {
    public:
        virtual int32_t
        GetStage()          { return mStage; }

    public:
        bool
        Compile(const char * source);

        inline uint32_t
        GetHash()           { return mHash; }

        inline uint32_t
        GetShaderObject()   { return mShader; }

    public:
        Shader(int32_t stage, IDeviceRef);
        ~Shader();

    private:
        const int32_t   mStage;
        uint32_t        mShader;
        uint32_t        mHash;
    };
    AUTOREF_REFERENCE_DECLARATION(Shader);

} // namespace GFW

#endif // __GFW_CORE_SHADER_H__
