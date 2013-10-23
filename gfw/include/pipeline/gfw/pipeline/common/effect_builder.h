#ifndef __GFW_PIPELINE_COMMON_EFFECT_BUILDER_H__
#define __GFW_PIPELINE_COMMON_EFFECT_BUILDER_H__

#include "common/autoref.h"
#include "common/string_table.h"
#include "gfw/pipeline/common/effect_binary.h"

#include <unordered_map>
#include <vector>

namespace GFW {

    class ParseTree;

    struct ShaderBinary
    {
        Common::InternedString  name;
    };
    typedef std::unordered_map< uint32_t, ShaderBinary > ShaderBinaryMap;
    typedef std::vector< ShaderBinary > ShaderBinaryVec;

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
        Common::StringTable &   mStringTable;
        TechniqueBinaryVec      mTechniques;
        ShaderBinaryVec         mShaders;
    };
    AUTOREF_REFERENCE_DECLARATION(EffectBuilder);

} // namespace GFW

#endif // __GFW_PIPELINE_COMMON_EFFECT_BUILDER_H__
