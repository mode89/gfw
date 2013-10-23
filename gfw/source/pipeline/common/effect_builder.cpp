#include "common/trace.h"

#include "gfw/pipeline/common/effect_builder.h"
#include "gfw/pipeline/common/parser.h"
#include "gfw/pipeline/common/parse_tree.h"

namespace GFW {

    using namespace Common;

    EffectBuilder::EffectBuilder( Common::StringTable & stringTable )
        : mStringTable( stringTable )
    {

    }

    EffectBinaryRef EffectBuilder::Build( const char * fileName )
    {
        ParserRef parser  = new Parser( fileName );

        const ParseTree * tree = parser->GetTree();

        // Process techniques

        tree->TraverseDFS( *this, &EffectBuilder::ProcessTechniques );

        return NULL;
    }

    bool EffectBuilder::ProcessTechniques( const ParseTree * tree )
    {
        if ( tree->GetTokenType() == TOKEN_EXTERNAL_DECLARATION )
        {
            const ParseTree * child = tree->GetChild();
            if ( child->GetTokenType() == TOKEN_TECHNIQUE_DEFINITION )
            {
                InternedString name = mStringTable.Resolve( child->GetChild()->ToString() );

                TechniqueBinary techBin;
                techBin.name = name;

                mTechniques.push_back( techBin );

                child->TraverseDFS( *this, &EffectBuilder::ProcessPasses );

                techBin.desc.passCount = techBin.passes.size();
            }
            return false;
        }
        return true;
    }

    bool EffectBuilder::ProcessPasses( const ParseTree * tree )
    {
        if ( tree->GetTokenType() == TOKEN_PASS_DEFINITION )
        {
            InternedString name = mStringTable.Resolve( tree->GetChild()->ToString() );

            PassBinary passBin;
            passBin.name = name;

            mTechniques.back().passes.push_back( passBin );

            tree->TraverseDFS( *this, &EffectBuilder::ProcessShaders );

            return false;
        }
        return true;
    }

    bool EffectBuilder::ProcessShaders( const ParseTree * tree )
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

            ShaderBinary shaderBin;
            shaderBin.name = name;

            mShaders.push_back( shaderBin );

            PassBinary & pass = mTechniques.back().passes.back();

            const ParseTree * shaderType = tree->GetChild( 0 );
            switch ( shaderType->GetTokenType() )
            {
            case TOKEN_SET_VERTEX_SHADER:
                pass.shader[ SHADER_STAGE_VERTEX ] = name;
                break;
            case TOKEN_SET_PIXEL_SHADER:
                pass.shader[ SHADER_STAGE_PIXEL ] = name;
                break;
            default:
                TRACE_FAIL();
                break;
            }
            return false;
        }
        return true;
    }

} // namespace GFW
