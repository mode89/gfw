#include "common/trace.h"

#include "gfw/pipeline/common/effect_builder.h"
#include "gfw/pipeline/common/parser.h"
#include "gfw/pipeline/common/parse_tree.h"

#include <unordered_map>

namespace GFW {

    using namespace Common;

    struct TechniqueBinary
    {
        TechniqueDesc   desc;
        InternedString  name;
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
                    TechniqueBinary techBin;
                    techBin.name = mStringTable.Resolve( child->GetChild()->ToString() );
                    mTechniques[ techBin.name.GetHash() ] = techBin;
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
