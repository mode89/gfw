#ifndef __GFW_PIPELINE_COMMON_EFFECT_BUILDER_H__
#define __GFW_PIPELINE_COMMON_EFFECT_BUILDER_H__

#include "common/autoref.h"
#include "common/string_table.h"
#include "gfw/pipeline/shader_builder.h"
#include "gfw/shared/effect_binary.h"

#include <unordered_map>
#include <vector>

namespace GFW {

    class ParseTree;

    struct PassBinary
    {
        Common::InternedString  name;
        Common::InternedString  shader[SHADER_STAGE_COUNT];
    };
    typedef std::unordered_map< uint32_t, PassBinary > PassBinaryMap;
    typedef std::vector< PassBinary > PassBinaryVec;

    struct TechniqueBinary
    {
        TechniqueDesc           desc;
        Common::InternedString  name;

        PassBinaryVec           passes;
    };
    typedef std::unordered_map< uint32_t, TechniqueBinary > TechniqueBinaryMap;
    typedef std::vector< TechniqueBinary > TechniqueBinaryVec;

    class EffectBuilder : public Common::ARefCounted
    {
    public:
        EffectBinaryRef
        Build( const char * fileName );

    public:
        EffectBuilder( Common::StringTable & );

    private:
        bool
        ProcessTechniques( const ParseTree * );

        bool
        ProcessPasses( const ParseTree * );

        bool
        ProcessShaders( const ParseTree * );

    private:
        typedef std::vector< ShaderBinaryRef > ShaderBinaryVec;

        Common::StringTable &   mStringTable;

        ShaderBuilderRef        mShaderBuilder;

        TechniqueBinaryVec      mTechniques;
        ShaderBinaryVec         mShaders;
    };
    AUTOREF_REFERENCE_DECLARATION(EffectBuilder);

} // namespace GFW

#endif // __GFW_PIPELINE_COMMON_EFFECT_BUILDER_H__
