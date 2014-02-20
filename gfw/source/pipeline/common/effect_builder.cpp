#include "common/trace.h"

#include "gfw/pipeline/common/effect_builder.h"
#include "gfw/pipeline/common/parse_tree.h"
#include "gfw/pipeline/shader_builder.h"

namespace GFW {

    using namespace Common;

    EffectBuilder::EffectBuilder()
    {

    }

    EffectBinaryRef EffectBuilder::Build( const char * fileName )
    {
        ConstParseTreeRef tree = CreateParseTree( fileName );

        mShaderBuilder = new ShaderBuilder( tree );

        EffectBinaryRef fxBin = new EffectBinary;

        mStringTable = new StringTable;

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

        fxBin->mStringTable = *mStringTable;
        mStringTable.Detach();

        return fxBin;
    }

    bool EffectBuilder::ProcessTechniques( ConstParseTreeIn tree )
    {
        if ( tree->GetTokenType() == T_EXTERNAL_DECLARATION )
        {
            ConstParseTreeRef child = tree->GetChild();
            if ( child->GetTokenType() == T_TECHNIQUE_DEFINITION )
            {
                InternedString name = mStringTable->Resolve( child->GetChild()->ToString() );

                TechniqueBinaryRef techBin = new TechniqueBinary;
                techBin->mName = name;

                mTechniques.push_back( techBin );

                mPasses.clear();
                child->TraverseDFS( *this, &EffectBuilder::ProcessPasses );

                techBin->mDesc.passCount = mPasses.size();
                techBin->mPasses = new PassBinaryRef [ mPasses.size() ];
                for ( PassBinaryVec::size_type i = 0; i < mPasses.size(); ++ i )
                {
                    techBin->mPasses[i] = mPasses[i];
                }
            }
            return false;
        }
        return true;
    }

    bool EffectBuilder::ProcessPasses( ConstParseTreeIn tree )
    {
        if ( tree->GetTokenType() == T_PASS_DEFINITION )
        {
            InternedString name = mStringTable->Resolve( tree->GetChild()->ToString() );

            PassBinaryRef passBin = new PassBinary;
            passBin->mName = name;

            mPasses.push_back( passBin );

            tree->TraverseDFS( *this, &EffectBuilder::ProcessShaders );

            return false;
        }
        return true;
    }

    bool EffectBuilder::ProcessShaders( ConstParseTreeIn tree )
    {
        if ( tree->GetTokenType() == T_SET_SHADER )
        {
            ConstParseTreeRef shader = tree->GetChild( 1 );

            InternedString name;
            InternedString profile;
            if ( shader->GetTokenType() == T_COMPILE_SHADER )
            {
                name = mStringTable->Resolve( shader->GetChild( 1 )->ToString() );
                profile = mStringTable->Resolve( shader->GetChild( 0 )->ToString() );
            }
            else
            {
                TRACE_FAIL();
            }

            ShaderBinaryRef shaderBin = mShaderBuilder->Compile( name.GetString(), profile.GetString() );

            uint32_t shaderIndex = mShaders.size();
            mShaders.push_back( shaderBin );

            PassBinaryRef pass = mPasses.back();

            ConstParseTreeRef shaderType = tree->GetChild( 0 );
            switch ( shaderType->GetTokenType() )
            {
            case T_SET_VERTEX_SHADER:
                pass->mShaders[ ShaderStage::VERTEX ] = shaderIndex;
                break;
            case T_SET_PIXEL_SHADER:
                pass->mShaders[ ShaderStage::PIXEL ] = shaderIndex;
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
