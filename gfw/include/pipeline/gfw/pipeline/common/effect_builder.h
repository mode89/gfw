#ifndef __GFW_PIPELINE_COMMON_EFFECT_BUILDER_H__
#define __GFW_PIPELINE_COMMON_EFFECT_BUILDER_H__

#include "common/autoref.h"
#include "common/string_table.h"
#include "gfw/pipeline/common/shader_builder.h"
#include "gfw/shared/effect.h"

#include <unordered_map>
#include <vector>

namespace GFW {

    class ParseTree;

    class EffectBuilder : public Common::ARefCounted
    {
    public:
        EffectBinaryRef
        Build( const char * fileName );

    public:
        EffectBuilder();

    private:
        bool
        ProcessTechniques( const ParseTree * );

        bool
        ProcessPasses( const ParseTree * );

        bool
        ProcessShaders( const ParseTree * );

    private:
        typedef std::vector< ShaderBinaryRef >    ShaderBinaryVec;
        typedef std::vector< TechniqueBinaryRef > TechniqueBinaryVec;
        typedef std::vector< PassBinaryRef >      PassBinaryVec;

        Common::StringTableRef  mStringTable;

        IShaderBuilderRef       mShaderBuilder;

        PassBinaryVec           mPasses;
        TechniqueBinaryVec      mTechniques;
        ShaderBinaryVec         mShaders;
    };
    AUTOREF_REFERENCE_DECLARATION(EffectBuilder);

} // namespace GFW

#endif // __GFW_PIPELINE_COMMON_EFFECT_BUILDER_H__
