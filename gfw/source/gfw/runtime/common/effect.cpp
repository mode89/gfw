#include "cmn/trace.h"
#include "gfw/device.h"
#include "gfw/runtime/common/effect.h"
#include "gfw/runtime/common/technique.h"
#include "gfw/shared/effect.h"
#include "gfw/shared/pass.h"
#include "gfw/shared/shader.h"
#include "gfw/shared/technique.h"

namespace GFW {

    Effect::Effect( const EffectBinary & effectBinary, IDeviceIn device )
    {
        ShaderTable shaderTable;

        mDesc.techniqueCount = static_cast< uint32_t >( effectBinary.mTechniques.size() );
        mTechniques.reserve( mDesc.techniqueCount );
        for ( auto techniqueBinary : effectBinary.mTechniques )
        {
            TechniqueRef technique = std::make_shared<Technique>( techniqueBinary, shaderTable, device );

            mTechniques.push_back( technique );
            mTechniqueMap[ techniqueBinary.mName ] = technique;
        }
    }

    Effect::~Effect()
    {

    }

    void Effect::Dispatch( uint32_t tech, uint32_t pass ) const
    {
        mTechniques[ tech ]->Dispatch( pass );
    }

    ConstITechniqueRef Effect::GetTechnique( const char * techName ) const
    {
        TechniqueMap::const_iterator it = mTechniqueMap.find( techName );
        CMN_ASSERT( it != mTechniqueMap.end() );
        return it->second;
    }

    ConstIShaderRef Effect::GetShader( ShaderStage stage, uint32_t tech, uint32_t pass ) const
    {
        return mTechniques[tech]->GetShader( stage, pass );
    }

} // namespace GFW
