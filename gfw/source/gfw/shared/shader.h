#ifndef __GFW_SHARED_SHADER_H__
#define __GFW_SHARED_SHADER_H__

#include <string>

namespace GFW {

    struct ShaderDesc
    {
        uint32_t    variableCount;
        uint32_t    bufferCount;
        uint32_t    resourceCount;
        uint32_t    inputsCount;

        ShaderDesc()
            : variableCount(0)
            , bufferCount(0)
            , resourceCount(0)
            , inputsCount(0)
        {}

        template < class Archive > void
        serialize( Archive & ar, unsigned version )
        {
            ar & variableCount;
            ar & bufferCount;
            ar & resourceCount;
            ar & inputsCount;
        }
    };

    class ShaderBinary
    {
        typedef std::string BinaryData;

    public:
        ShaderDesc  mDesc;
        uint32_t    mStage;
        BinaryData  mData;

        template < class Archive > void
        serialize( Archive & ar, unsigned version )
        {
            ar & mDesc;
            ar & mStage;
            ar & mData;
        }
    };

} // namespace GFW

#endif // __GFW_SHARED_SHADER_H__
