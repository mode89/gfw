#include "common/trace.h"

#include "gfw/pipeline/common/effect_builder.h"
#include "gfw/pipeline/common/parser.h"
#include "gfw/pipeline/common/parse_tree.h"

#include <unordered_map>

namespace GFW {

    using namespace Common;

    struct ShaderBinary
    {
        InternedString  name;
    };
    typedef std::unordered_map< uint32_t, ShaderBinary > ShaderBinaryMap;

    class CollectShadersVisitor
    {
    public:
        CollectShadersVisitor( InternedString * passShaders, ShaderBinaryMap & shaders, StringTable & stringTable )
            : mPassShaders( passShaders )
            , mShaders( shaders )
            , mStringTable( stringTable )
        {}

        bool operator() ( const ParseTree * tree )
        {
            if ( tree->GetTokenType() == TOKEN_SET_SHADER )
            {
                const ParseTree * shader = tree->GetChild( 1 );

                InternedString name;
                if ( shader->GetTokenType() == TOKEN_COMPILE_SHADER )
                {
                    name = mStringTable.Resolve( shader->GetChild( 1 )->ToString() );
                }
                else
                {
                    TRACE_FAIL();
                }

                ShaderBinary & shaderBin = mShaders[ name.GetHash() ];
                shaderBin.name = name;

                const ParseTree * shaderType = tree->GetChild( 0 );
                switch ( shaderType->GetTokenType() )
                {
                case TOKEN_SET_VERTEX_SHADER:
                    mPassShaders[ SHADER_STAGE_VERTEX ] = name;
                    break;
                case TOKEN_SET_PIXEL_SHADER:
                    mPassShaders[ SHADER_STAGE_PIXEL ] = name;
                    break;
                default:
                    TRACE_FAIL();
                    break;
                }
                return false;
            }
            return true;
        }

    private:
        InternedString *    mPassShaders;
        ShaderBinaryMap &   mShaders;
        StringTable &       mStringTable;
    };

    struct PassBinary
    {
        InternedString  name;
        InternedString  shader[SHADER_STAGE_COUNT];
    };
    typedef std::unordered_map< uint32_t, PassBinary > PassBinaryMap;

    class CollectPassesVisitor
    {
    public:
        CollectPassesVisitor( ShaderBinaryMap & shaders, PassBinaryMap & passes, StringTable & stringTable )
            : mShaders( shaders )
            , mPasses( passes )
            , mStringTable( stringTable )
        {}

        bool operator() ( const ParseTree * tree )
        {
            if ( tree->GetTokenType() == TOKEN_PASS_DEFINITION )
            {
                InternedString name = mStringTable.Resolve( tree->GetChild()->ToString() );
                PassBinary & passBin = mPasses[ name.GetHash() ];
                passBin.name = name;

                CollectShadersVisitor collectShaders( passBin.shader, mShaders, mStringTable );
                tree->TraverseDFS( collectShaders );

                return false;
            }
            return true;
        }

    private:
        ShaderBinaryMap &   mShaders;
        PassBinaryMap &     mPasses;
        StringTable &       mStringTable;
    };

    struct TechniqueBinary
    {
        TechniqueDesc   desc;
        InternedString  name;

        PassBinaryMap   passes;
    };
    typedef std::unordered_map< uint32_t, TechniqueBinary > TechniqueBinaryMap;

    class CollectTechniquesVisitor
    {
    public:
        CollectTechniquesVisitor( ShaderBinaryMap & shaders, TechniqueBinaryMap & techniques, StringTable & stringTable )
            : mShaders( shaders )
            , mTechniques( techniques )
            , mStringTable( stringTable )
        {}

        bool operator() ( const ParseTree * tree )
        {
            if ( tree->GetTokenType() == TOKEN_EXTERNAL_DECLARATION )
            {
                const ParseTree * child = tree->GetChild();
                if ( child->GetTokenType() == TOKEN_TECHNIQUE_DEFINITION )
                {
                    InternedString name = mStringTable.Resolve( child->GetChild()->ToString() );
                    TechniqueBinary & techBin = mTechniques[ name.GetHash() ];
                    techBin.name = name;

                    CollectPassesVisitor collectPasses( mShaders, techBin.passes, mStringTable );
                    child->TraverseDFS( collectPasses );

                    techBin.desc.passCount = techBin.passes.size();
                }
                return false;
            }
            return true;
        }

    private:
        ShaderBinaryMap &       mShaders;
        TechniqueBinaryMap &    mTechniques;
        StringTable &           mStringTable;
    };

    EffectBinaryRef EffectBuilder::Build( const char * fileName, StringTable & stringTable )
    {
        ParserRef parser  = new Parser( fileName );

        const ParseTree * tree = parser->GetTree();

        // Collect techniques

        ShaderBinaryMap    shaders;
        TechniqueBinaryMap techniques;
        CollectTechniquesVisitor collectTechniques( shaders, techniques, stringTable );
        tree->TraverseDFS( collectTechniques );

        return NULL;
    }

} // namespace GFW
