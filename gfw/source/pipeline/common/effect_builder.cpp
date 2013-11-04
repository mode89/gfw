#include "common/trace.h"

#include "gfw/pipeline/common/effect_builder.h"
#include "gfw/pipeline/common/parser.h"
#include "gfw/pipeline/common/parse_tree.h"
#include "gfw/pipeline/shader_builder.h"

namespace GFW {

    using namespace Common;

    EffectBuilder::EffectBuilder()
    {

    }

    EffectBinaryRef EffectBuilder::Build( const char * fileName )
    {
        ParserRef parser  = new Parser( fileName );

        const ParseTree * tree = parser->GetTree();

        mShaderBuilder = new ShaderBuilder( tree );

        EffectBinaryRef fxBin = new EffectBinary;

        // Process techniques

        tree->TraverseDFS( *this, &EffectBuilder::ProcessTechniques );

        // Save techniques to the binary

        fxBin->mDesc.techniqueCount = mTechniques.size();
        fxBin->mTechniques = new TechniqueBinaryRef [ mTechniques.size() ];
        for ( uint32_t i = 0; i < mTechniques.size(); ++ i )
        {
            fxBin->mTechniques[i] = mTechniques[i];
        }

        // Save shaders to the binary

        fxBin->mShaderCount = mShaders.size();
        fxBin->mShaders = new ShaderBinaryRef [ mShaders.size() ];
        for ( uint32_t i = 0; i < mShaders.size(); ++ i )
        {
            fxBin->mShaders[i] = mShaders[i];
        }

        return fxBin;
    }

    bool EffectBuilder::ProcessTechniques( const ParseTree * tree )
    {
        if ( tree->GetTokenType() == TOKEN_EXTERNAL_DECLARATION )
        {
            const ParseTree * child = tree->GetChild();
            if ( child->GetTokenType() == TOKEN_TECHNIQUE_DEFINITION )
            {
                InternedString name = mStringTable.Resolve( child->GetChild()->ToString() );

                TechniqueBinaryRef techBin = new TechniqueBinary;
                techBin->mName = name;

                mTechniques.push_back( techBin );

                child->TraverseDFS( *this, &EffectBuilder::ProcessPasses );

                techBin->mDesc.passCount = techBin->mPasses.size();
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

            PassBinaryRef passBin = new PassBinary;
            passBin->mName = name;

            mTechniques.back()->mPasses.push_back( passBin );

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
            InternedString profile;
            if ( shader->GetTokenType() == TOKEN_COMPILE_SHADER )
            {
                name = mStringTable.Resolve( shader->GetChild( 1 )->ToString() );
                profile = mStringTable.Resolve( shader->GetChild( 0 )->ToString() );
            }
            else
            {
                TRACE_FAIL();
            }

            ShaderBinaryRef shaderBin = mShaderBuilder->Compile( name.GetString(), profile.GetString() );

            mShaders.push_back( shaderBin );

            PassBinaryRef pass = mTechniques.back()->mPasses.back();

            const ParseTree * shaderType = tree->GetChild( 0 );
            switch ( shaderType->GetTokenType() )
            {
            case TOKEN_SET_VERTEX_SHADER:
                pass->mShaders[ SHADER_STAGE_VERTEX ] = name;
                break;
            case TOKEN_SET_PIXEL_SHADER:
                pass->mShaders[ SHADER_STAGE_PIXEL ] = name;
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
