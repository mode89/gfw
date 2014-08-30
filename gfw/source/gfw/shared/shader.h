#ifndef __GFW_SHARED_SHADER_H__
#define __GFW_SHARED_SHADER_H__

#include "gfw/shader_reflect.h"
#include <string>
#include <list>

namespace GFW {

    struct ShaderResourceBinary
    {
        std::string     name;
        uint32_t        bindSlot;
        uint32_t        bindCount;
        uint32_t        type;
        uint32_t        dim;

        ShaderResourceBinary()
            : name()
            , bindSlot( 0 )
            , bindCount( 0 )
            , type( SHADER_RES_TYPE_UNKNOWN )
            , dim( SHADER_RES_DIM_UNKNOWN )
        {}

        template < class Archive > void
        serialize( Archive & ar, unsigned version )
        {
            ar & name;
            ar & bindSlot;
            ar & bindCount;
            ar & type;
            ar & dim;
        }
    };

    class ShaderBinary
    {
        typedef std::string BinaryData;
        typedef std::list< ShaderResourceBinary > ShaderResourceBinaryVec;

    public:
        uint32_t                    mStage;
        BinaryData                  mData;
        ShaderResourceBinaryVec     mResources;

        template < class Archive > void
        serialize( Archive & ar, unsigned version )
        {
            ar & mStage;
            ar & mData;
            ar & mResources;
        }
    };

} // namespace GFW

#endif // __GFW_SHARED_SHADER_H__
