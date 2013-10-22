#include "common/trace.h"

#include "gfw/pipeline/common/effect_builder.h"
#include "gfw/pipeline/common/parser.h"
#include "gfw/pipeline/common/parse_tree.h"

#include <unordered_map>

namespace GFW {

    using namespace Common;

    struct PassBinary
    {
        InternedString  name;
    };
    typedef std::unordered_map< uint32_t, PassBinary > PassBinaryMap;

    class CollectPassesVisitor
    {
    public:
        CollectPassesVisitor( PassBinaryMap & passes, StringTable & stringTable )
            : mPasses( passes )
            , mStringTable( stringTable )
        {}

        bool operator() ( const ParseTree * tree )
        {
            if ( tree->GetTokenType() == TOKEN_PASS_DEFINITION )
            {
                InternedString name = mStringTable.Resolve( tree->GetChild()->ToString() );
                PassBinary & passBin = mPasses[ name.GetHash() ];
                passBin.name = name;
                return false;
            }
            return true;
        }

    private:
        PassBinaryMap & mPasses;
        StringTable &   mStringTable;
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
        CollectTechniquesVisitor( TechniqueBinaryMap & techniques, StringTable & stringTable )
            : mTechniques( techniques )
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

                    CollectPassesVisitor collectPasses( techBin.passes, mStringTable );
                    child->TraverseDFS( collectPasses );

                    techBin.desc.passCount = techBin.passes.size();
                }
                return false;
            }
            return true;
        }

    private:
        TechniqueBinaryMap &    mTechniques;
        StringTable &           mStringTable;
    };

    EffectBinaryRef EffectBuilder::Build( const char * fileName, StringTable & stringTable )
    {
        ParserRef parser  = new Parser( fileName );

        const ParseTree * tree = parser->GetTree();

        // Collect techniques

        TechniqueBinaryMap techniques;
        CollectTechniquesVisitor collectTechniques( techniques, stringTable );
        tree->TraverseDFS( collectTechniques );

        return NULL;
    }

} // namespace GFW
