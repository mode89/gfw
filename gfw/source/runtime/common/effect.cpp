#include "common/trace.h"

#include "gfw/base/context.h"

#include "gfw/runtime/common/effect.h"
#include "gfw/runtime/common/device_child.inl"
#include "gfw/runtime/common/technique.h"
#include "gfw/runtime/core/device.h"

namespace GFW {

    using namespace Common;

    Effect::Effect( EffectBinaryRef effectBinary, IDeviceIn device )
        : ADeviceChild(device)
        , mDesc( effectBinary->mDesc )
    {
        StringTable & stringTable = device.StaticCast<Device>()->GetStringTable();
        stringTable.Resolve( effectBinary->mStringTable );

        mShaders.reserve( effectBinary->mShaderCount );
        for ( uint32_t i = 0; i < effectBinary->mShaderCount; ++ i )
        {
            ShaderBinaryRef shaderBin = effectBinary->mShaders[i];
            ShaderStage stage = static_cast<ShaderStage>( shaderBin->mStage );
            mShaders.push_back( device->CreateShader( stage, shaderBin ) );
        }

        mTechniques.reserve( mDesc.techniqueCount );
        for ( uint32_t i = 0; i < mDesc.techniqueCount; ++ i )
        {
            TechniqueBinaryRef techBin = effectBinary->mTechniques[i];
            ITechniqueRef tech = new Technique( techBin, this );
            InternedString name = stringTable.Resolve( techBin->mName );

            mTechniques.push_back( tech );
            mTechniqueMap[ name.GetString() ] = tech;
        }
    }

    Effect::~Effect()
    {

    }

    void Effect::Dispatch( uint32_t tech /* = 0 */, uint32_t pass /* = 0 */ )
    {
        IContextRef context = mDevice->GetCurrentContext();
        TRACE_ASSERT(context.IsAttached());

        mTechniques[ tech ]->Dispatch( pass );
    }

    ITechniqueRef Effect::GetTechnique( const char * techName )
    {
        TechniqueMap::iterator it = mTechniqueMap.find( techName );
        TRACE_ASSERT( it != mTechniqueMap.end() );
        return it->second;
    }

    IShaderRef Effect::GetShader( ShaderStage stage, uint32_t tech /* = 0 */, uint32_t pass /* = 0 */ )
    {
        return mTechniques[tech]->GetShader( stage, pass );
    }

} // namespace GFW
