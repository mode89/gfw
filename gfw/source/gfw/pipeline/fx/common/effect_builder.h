#ifndef __GFW_PIPELINE_FX_COMMON_EFFECT_BUILDER_H__
#define __GFW_PIPELINE_FX_COMMON_EFFECT_BUILDER_H__

CMN_WARNING_PUSH
CMN_WARNING_DISABLE_MSVC( 4625 4626 4640 )
#include "boost/filesystem.hpp"
CMN_WARNING_POP

#include <list>
#include <memory>

namespace GFW {

    class IShaderBuilder;

    class EffectBinary;
    class ShaderBinary;
    class ShaderTable;
    class ParseTree;

    class EffectBuilder
    {
    public:
        void
        Build( EffectBinary &, const boost::filesystem::path & fileName );

    public:
        EffectBuilder();

    private:
        bool
        ProcessTechniques( const ParseTree & );

        bool
        ProcessPasses( const ParseTree & );

        bool
        ProcessShaders( const ParseTree & );

    private:
        typedef std::list< std::shared_ptr< ShaderBinary > > ShaderTable;

        EffectBinary *                      mEffectBinary;
        std::weak_ptr< IShaderBuilder >     mShaderBuilder;
        std::weak_ptr< ShaderTable >        mShaderTable;
    };

} // namespace GFW

#endif // __GFW_PIPELINE_FX_COMMON_EFFECT_BUILDER_H__
