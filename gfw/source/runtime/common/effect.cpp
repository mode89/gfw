#include "common/trace.h"

#include "gfw/base/context.h"

#include "gfw/runtime/common/effect.h"
#include "gfw/runtime/common/device_child.inl"
#include "gfw/runtime/common/technique.h"

namespace GFW {

    Effect::Effect( EffectBinaryRef effectBinary, IDeviceIn device )
        : ADeviceChild(device)
        , mDesc( effectBinary->mDesc )
    {
        mShaders.reserve( effectBinary->mShaderCount );
        for ( uint32_t i = 0; i < effectBinary->mShaderCount; ++ i )
        {
            ShaderBinaryRef shaderBin = effectBinary->mShaders[i];
            ShaderStage stage = static_cast<ShaderStage>( shaderBin->mDesc.stage );
            mShaders.push_back( device->CreateShader( stage, shaderBin ) );
        }

        mTechniques.reserve( mDesc.techniqueCount );
        for ( uint32_t i = 0; i < mDesc.techniqueCount; ++ i )
        {
            TechniqueBinaryRef techBin = effectBinary->mTechniques[i];
            ITechniqueRef tech = new Technique( techBin );
            mTechniques.push_back( tech );
            //mTechniqueMap[ techBin->mName.GetString() ] = tech;
        }
    }

    Effect::~Effect()
    {

    }

    void Effect::Dispatch( uint32_t tech /* = 0 */, uint32_t pass /* = 0 */ )
    {
        IContextRef context = mDevice->GetCurrentContext();
        TRACE_ASSERT(context.IsAttached());

        TRACE_FAIL_MSG( "Not yet implemented" );
    }

    ITechniqueRef Effect::GetTechnique( const char * techName )
    {
        TechniqueMap::iterator it = mTechniqueMap.find( techName );
        TRACE_ASSERT( it != mTechniqueMap.end() );
        return it->second;
    }

    IShaderRef Effect::GetShader( ShaderStage stage, uint32_t tech /* = 0 */, uint32_t pass /* = 0 */ )
    {
        TRACE_FAIL_MSG( "Not yet implmented" );
        return NULL;
    }

} // namespace GFW
