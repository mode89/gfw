#include "cmn/trace.h"
#include "cmn/crc32.h"
#include "gfw/texture_utils.h"
#include "gfw/runtime/common/device_child.inl"
#include "gfw/runtime/ogl4/buffer.h"
#include "gfw/runtime/ogl4/context.h"
#include "gfw/runtime/ogl4/device.h"
#include "gfw/runtime/ogl4/format.h"
#include "gfw/runtime/ogl4/functions.h"
#include "gfw/runtime/ogl4/input_layout.h"
#include "gfw/runtime/ogl4/render_target.h"
#include "gfw/runtime/ogl4/shader.h"
#include "gfw/runtime/ogl4/shader_reflect.h"
#include "gfw/runtime/ogl4/shader_stage.h"
#include "gfw/runtime/ogl4/texture.h"

namespace GFW {

    using namespace Cmn;

    Context::Context( NativeContextIn nativeContext, DeviceIn d )
        : ADeviceChild( d )
        , mNativeContext( nativeContext )
        , mScreenQuadBuffer( 0 )
        , mDirtyFlags()
        , mShaders()
        , mProgramPipeline( 0 )
        , mInputLayout()
        , mEnabledVertexAttributesMask( 0 )
        , mVertexBuffers()
        , mIndexBuffer()
        // , mTextures()
        , mResidentTextureHandles()
        , mRenderTargets()
        , mRenderTargetsCount( 0 )
        , mDrawFramebuffer( 0 )
    {
        VGL( glGenProgramPipelines, 1, &mProgramPipeline );
        CMN_ASSERT( mProgramPipeline != 0 );
        VGL( glBindProgramPipeline, mProgramPipeline );

        VGL( glGenFramebuffers, 1, &mDrawFramebuffer );
        CMN_ASSERT( mDrawFramebuffer != 0 );
        VGL( glBindFramebuffer, GL_DRAW_FRAMEBUFFER, mDrawFramebuffer );

        InitScreenQuad();

        ClearState();
    }

    Context::~Context()
    {
        if (mProgramPipeline != 0)
        {
            VGL( glDeleteProgramPipelines, 1, &mProgramPipeline );
        }

        if (mDrawFramebuffer != 0)
        {
            VGL( glDeleteFramebuffers, 1, &mDrawFramebuffer );
        }

        if (mScreenQuadBuffer)
        {
            VGL( glDeleteBuffers, 1, &mScreenQuadBuffer );
        }
    }

    void Context::InitScreenQuad()
    {
        static const float vertices[] = {
            -1.0f, -1.0f,
            -1.0f,  1.0f,
             1.0f, -1.0f,
             1.0f,  1.0f
        };

        VGL( glGenBuffers, 1, &mScreenQuadBuffer );
        CMN_ASSERT( mScreenQuadBuffer != 0 );
        VGL( glBindBuffer, GL_ARRAY_BUFFER, mScreenQuadBuffer );
        VGL( glBufferData, GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW );
        VGL( glBindBuffer, GL_ARRAY_BUFFER, 0 );
    }

    void Context::SetInputLayout( ConstIInputLayoutIn layout )
    {
        mInputLayout = std::static_pointer_cast< const InputLayout >( layout );
    }

    void Context::SetVertexBuffer( uint32_t slot, ConstIBufferIn buf )
    {
        CMN_ASSERT( slot < MAX_BIND_VERTEX_BUFFERS );
        CMN_ASSERT( buf );

        mVertexBuffers[ slot ] = std::static_pointer_cast< const Buffer >( buf );
    }

    void Context::SetShader( ShaderStage stage, ConstIShaderIn shader )
    {
        CMN_ASSERT( stage > SHADER_STAGE_UNKNOWN );
        CMN_ASSERT( stage < SHADER_STAGE_COUNT );
        CMN_ASSERT( shader );
        CMN_ASSERT( stage == shader->GetStage() );

        mShaders[ stage ] = std::static_pointer_cast< const Shader >( shader );
    }

    void Context::Clear(const ClearParams & cp)
    {
        uint32_t mask = 0;

        if ( cp.mask & CLEAR_COLOR )
        {
            VGL( glClearColor, cp.color[ 0 ], cp.color[ 1 ], cp.color[ 2 ], cp.color[ 3 ] );
            mask |= GL_COLOR_BUFFER_BIT;
        }

		if ( cp.mask & CLEAR_DEPTH )
		{
			VGL( glClearDepth, cp.depth );
			mask |= GL_DEPTH_BUFFER_BIT;
		}

        FlushState();

        VGL( glClear, mask );
    }

    void Context::DrawScreenQuad()
    {
        FlushState();

        VGL( glBindBuffer, GL_ARRAY_BUFFER, mScreenQuadBuffer );
        VGL( glVertexAttribPointer, 0, 2, GL_FLOAT, GL_TRUE, 2 * sizeof( float ), nullptr );
        VGL( glEnableVertexAttribArray, 0 );
        VGL( glBindBuffer, GL_ARRAY_BUFFER, 0 );

        VGL( glDrawArrays, GL_TRIANGLE_STRIP, 0, 4 );
    }

    void Context::Draw( const DrawParams & dp )
    {
        FlushState();

        uint32_t mode = GetOGLDrawMode( dp.primTop );
        VGL( glDrawArrays, mode, dp.vertexStart, dp.vertexCount );
    }

    void Context::Draw( const DrawIndexedParams & dp )
    {
        FlushState();

        uint32_t mode     = GetOGLDrawMode(dp.primTop);
        uint32_t type     = GetOGLType(dp.indexType);
        size_t   typeSize = GetTypeSize(dp.indexType);

        VGL( glDrawElements, mode, dp.indexCount, type, reinterpret_cast< void* >( typeSize * dp.indexStart ) );
    }

    void Context::ClearState()
    {
        // Detach shaders

        for (int stage = 0; stage < SHADER_STAGE_COUNT; ++ stage)
        {
            uint32_t stageBit = GetOGLShaderStageBit( static_cast< ShaderStage >( stage ) );
            VGL( glUseProgramStages, mProgramPipeline, stageBit, 0 );

            mShaders[ stage ].reset();
        }

        // Detach buffers

        for (uint32_t i = 0; i < MAX_BIND_VERTEX_BUFFERS; ++ i)
        {
            mVertexBuffers[ i ].reset();
        }

        VGL( glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, 0 );
        mIndexBuffer.reset();

        for (uint32_t attrIndex = 0, mask = mEnabledVertexAttributesMask; mask; ++ attrIndex, mask >>= 1)
        {
            if (mask & 1)
            {
                CMN_ASSERT( attrIndex < MAX_INPUT_ATTRIBUTES );
                VGL( glDisableVertexAttribArray, attrIndex );
            }
        }
        mEnabledVertexAttributesMask = 0;

        mInputLayout.reset();

        // Detach constant shaders

            for ( unsigned stage = 0; stage < SHADER_STAGE_COUNT; ++ stage )
            {
                for ( unsigned slot = 0; slot < MAX_BIND_CBUFFERS; ++ slot )
                {
                    if ( mCBuffers[ stage ][ slot ] ||
                         mDirtyFlags.cbuffers[ stage ] & ( 1 << slot ) )
                    {
                        VGL( glBindBufferBase, GL_UNIFORM_BUFFER, stage * MAX_BIND_CBUFFERS + slot, 0 );
                        mCBuffers[ stage ][ slot ].reset();
                    }
                }
                mDirtyFlags.cbuffers[ stage ] = 0;
            }

        // Detach textures

            for ( uint32_t stage = 0; stage < SHADER_STAGE_COUNT; ++ stage )
            {
                for ( uint32_t slot = 0; slot < MAX_BIND_TEXTURES; ++ slot )
                {
                    mTextures[ stage ][ slot ].reset();
                }
            }

            for ( auto it : mResidentTextureHandles )
            {
                VGL( glMakeTextureHandleNonResidentARB, it.second );
            }
            mResidentTextureHandles.clear();

        // Detach render targets

        if ( mRenderTargetsCount )
        {
            for ( uint32_t i = 0; i < mRenderTargetsCount; ++ i )
            {
                mRenderTargets[ i ].reset();
            }
            mRenderTargetsCount = 0;

            mDirtyFlags.renderTargets = true;
        }
    }

    void Context::FlushState()
    {
        // Setup shaders

        VGL( glBindProgramPipeline, mProgramPipeline );
        for ( int stage = 0; stage < SHADER_STAGE_COUNT; ++ stage )
        {
            ConstShaderRef shader = mShaders[ stage ];
            uint32_t stageBit = GetOGLShaderStageBit( static_cast< ShaderStage >( stage ) );
            VGL( glUseProgramStages, mProgramPipeline, stageBit, shader ? shader->GetHandle() : 0 );
        }

        // Bind buffers

        if ( mInputLayout )
        {
            CMN_ASSERT( std::static_pointer_cast< const ShaderReflection >(
                mShaders[ SHADER_STAGE_VERTEX ]->GetReflection() )->GetInputSignature()
                    == mInputLayout->GetInputSignature() );

            for ( uint32_t attrIndex = 0, mask = mInputLayout->GetEnabledAttributesMask(); mask; ++ attrIndex, mask >>= 1 )
            {
                if ( mask & 1 )
                {
                    CMN_ASSERT( attrIndex < MAX_INPUT_ATTRIBUTES );

                    const VertexAttribute & attr = mInputLayout->GetAttribute( attrIndex );
                    CMN_ASSERT( attr.semantic != SEMANTIC_UNKNOWN );
                    CMN_ASSERT( mVertexBuffers[attr.bufSlot] );

                    VGL( glBindBuffer, GL_ARRAY_BUFFER, mVertexBuffers[ attr.bufSlot ]->GetHandle() );

                    int32_t size = GetFormatElementNumber( attr.format );
                    Type type    = GetFormatElementType( attr.format );
                    VGL( glVertexAttribPointer, attrIndex, size, GetOGLType( type ), GL_FALSE, attr.stride,
                        reinterpret_cast< void* >( static_cast< uint64_t >( attr.offset ) ) );
                    VGL( glEnableVertexAttribArray, attrIndex );

                    VGL( glBindBuffer, GL_ARRAY_BUFFER, 0 );
                }
            }

            for (uint32_t attrIndex = 0, mask = ( mEnabledVertexAttributesMask & !mInputLayout->GetEnabledAttributesMask() ); mask;
                ++ attrIndex, mask >>= 1)
            {
                if (mask & 1)
                {
                    VGL( glDisableVertexAttribArray, attrIndex );
                }
            }
            mEnabledVertexAttributesMask = mInputLayout->GetEnabledAttributesMask();

            if ( mIndexBuffer )
            {
                VGL( glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer->GetHandle() );
            }
        }

        // Bind constant buffers

            for ( unsigned stage = 0; stage < SHADER_STAGE_COUNT; ++ stage )
            {
                for ( unsigned slot = 0, mask = mDirtyFlags.cbuffers[ stage ]; mask; ++ slot, mask >>= 1 )
                {
                    if ( mask & 1u )
                    {
                        ConstBufferRef buffer = mCBuffers[ stage ][ slot ];
                        uint32_t handle = buffer ? buffer->GetHandle() : 0;
                        VGL( glBindBufferBase, GL_UNIFORM_BUFFER,
                            stage * MAX_BIND_CBUFFERS + slot, handle );
                    }
                }
                mDirtyFlags.cbuffers[ stage ] = 0;
            }

        // Bind textures

            for ( uint32_t stage = 0; stage < SHADER_STAGE_COUNT; ++ stage )
            {
                if ( mShaders[ stage ] )
                {
                    ConstShaderReflectionRef reflection =
                        std::static_pointer_cast< const ShaderReflection >(
                            mShaders[ stage ]->GetReflection() );
                    for ( auto & textureSampler : reflection->GetTextureSamplers() )
                    {
                        uint32_t textureSlot = textureSampler.texture;
                        if ( mTextures[ stage ][ textureSlot ] )
                        {
                            ConstTextureRef texture = mTextures[ stage ][ textureSlot ];
                            uint32_t program = mShaders[ stage ]->GetHandle();
                            uint64_t handle = VGL( glGetTextureHandleARB, texture->GetHandle() );

                            VGL( glMakeTextureHandleResidentARB, handle );
                            mResidentTextureHandles[ texture.get() ] = handle;

                            VGL( glProgramUniformHandleui64ARB, program, textureSampler.location, handle );
                        }
                    }
                }
            }

        // Bind render targets

        if ( mDirtyFlags.renderTargets )
        {
            if ( mRenderTargetsCount > 0 )
            {
                VGL( glBindFramebuffer, GL_DRAW_FRAMEBUFFER, mDrawFramebuffer );

                uint32_t drawBuffers[ MAX_RENDER_TARGETS ];
                for (uint32_t i = 0; i < mRenderTargetsCount; ++ i)
                {
                    ConstRenderTargetRef rt = mRenderTargets[ i ];
                    ConstTextureRef rtTex   = std::static_pointer_cast< const Texture >( rt->GetTexture() );
                    VGL( glFramebufferTexture, GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, rtTex->GetHandle(), rt->GetDesc().resourceIndex );
                    drawBuffers[ i ] = GL_COLOR_ATTACHMENT0 + i;
                }
                VGL( glDrawBuffers, mRenderTargetsCount, drawBuffers );

#ifdef CMN_DEBUG
                int32_t status  = VGL( glCheckFramebufferStatus, GL_DRAW_FRAMEBUFFER );
                CMN_ASSERT( status == GL_FRAMEBUFFER_COMPLETE );
#endif
            }
            else
            {
                VGL( glBindFramebuffer, GL_DRAW_FRAMEBUFFER, 0 );
            }

            mDirtyFlags.renderTargets = false;
        }
    }

    void Context::SetIndexBuffer( ConstIBufferIn buffer )
    {
        mIndexBuffer = std::static_pointer_cast< const Buffer >( buffer );
    }

    void Context::SetConstantBuffer( ShaderStage stage, uint32_t slot, ConstIBufferIn buffer )
    {
        CMN_ASSERT( stage >= 0 );
        CMN_ASSERT( stage < SHADER_STAGE_COUNT );
        CMN_ASSERT( slot < MAX_BIND_CBUFFERS );

        mCBuffers[ stage ][ slot ] = std::static_pointer_cast< const Buffer >( buffer );
        mDirtyFlags.cbuffers[ stage ] |= ( 1 << slot );
    }

    void Context::SetTexture( ShaderStage stage, uint32_t slot, ConstITextureIn texture )
    {
        CMN_ASSERT( stage >= 0 );
        CMN_ASSERT( stage < SHADER_STAGE_COUNT );
        CMN_ASSERT( slot < MAX_BIND_TEXTURES );
        CMN_ASSERT( texture );

        if ( mTextures[ stage ][ slot ] )
        {
            ConstTextureRef texture = mTextures[ stage ][ slot ];
            for ( auto it = mResidentTextureHandles.begin(); it != mResidentTextureHandles.end(); )
            {
                if ( it->first == texture.get() )
                {
                    VGL( glMakeTextureHandleNonResidentARB, it->second );
                    mResidentTextureHandles.erase( it ++ );
                }
                else
                {
                    ++ it;
                }
            }
        }

        mTextures[ stage ][ slot ] = std::static_pointer_cast< const Texture >( texture );
    }

    void Context::SetRenderTargets( uint32_t count, ConstIRenderTargetRef rt[] )
    {
        for ( uint32_t i = 0; i < count; ++ i )
        {
            mRenderTargets[ i ] = std::static_pointer_cast< const RenderTarget >( rt[ i ] );
        }

        for ( uint32_t i = count; i < mRenderTargetsCount; ++ i )
        {
            mRenderTargets[ i ].reset();
        }

        mRenderTargetsCount = count;

        mDirtyFlags.renderTargets = true;
    }

    void Context::BeginScene()
    {
        mDevice.lock()->LockContext( shared_from_this() );
    }

    void Context::EndScene()
    {
        ClearState();
        mDevice.lock()->UnlockContext( shared_from_this() );
    }

    void Context::Map(
        IResourceIn resource,
        const SubResourceIndex & index,
        MapType mapType,
        SubResourceData & mappedData )
    {
        CMN_ASSERT( resource->GetDesc().cpuAccessFlags != 0 );  // resource cannot be mapped
        CMN_ASSERT( ( mapType == MAP_TYPE_READ ) ?              // resource cannot be read
            ( resource->GetDesc().cpuAccessFlags & CPU_ACCESS_FLAG_READ ) : 1 );
        CMN_ASSERT( ( mapType == MAP_TYPE_WRITE ) ?             // resource cannot be written
            ( resource->GetDesc().cpuAccessFlags & CPU_ACCESS_FLAG_WRITE ) : 1 );
        CMN_ASSERT( ( mapType == MAP_TYPE_READ_WRITE ) ?        // resource cannot be read and written
            ( resource->GetDesc().cpuAccessFlags & CPU_ACCESS_FLAG_READ ) &&
            ( resource->GetDesc().cpuAccessFlags & CPU_ACCESS_FLAG_WRITE ) : 1 );

        switch ( resource->GetType() )
        {
        case RESOURCE_TYPE_BUFFER:
            {
                BufferRef buffer = std::static_pointer_cast< Buffer >( resource );
                buffer->Map( index, mapType, mappedData );
            }
            return;
        case RESOURCE_TYPE_TEXTURE:
            {
                TextureRef texture = std::static_pointer_cast< Texture >( resource );
                const TextureDesc & desc = texture->GetDesc();

                VGL( glBindBuffer, GL_PIXEL_UNPACK_BUFFER, texture->GetBufferHandle() );
                {
                    uint32_t offset = GetTextureMipOffset( desc.format, desc.width, desc.height, index.mipSlice );
                    uint32_t size = GetTextureMipSize( desc.format, desc.width, desc.height, index.mipSlice );
                    uint32_t access = 0;
                    switch ( mapType )
                    {
                    case MAP_TYPE_READ:
                        access = GL_MAP_READ_BIT;
                        break;
                    case MAP_TYPE_WRITE:
                        access = GL_MAP_WRITE_BIT;
                        break;
                    case MAP_TYPE_READ_WRITE:
                        access = GL_MAP_READ_BIT | GL_MAP_WRITE_BIT;
                        break;
                    default:
                        CMN_FAIL();
                    }
                    mappedData.mem = VGL( glMapBufferRange, GL_PIXEL_UNPACK_BUFFER, offset, size, access );
                    mappedData.slicePitch = size;
                    mappedData.rowPitch = GetTextureMipPitch( desc.format, desc.width, index.mipSlice );
                }
                VGL( glBindBuffer, GL_PIXEL_UNPACK_BUFFER, 0 );
            }
            return;
        default:
            CMN_FAIL();
        }
    }

    void Context::Unmap( IResourceIn resource, const SubResourceIndex & index )
    {
        switch ( resource->GetType() )
        {
        case RESOURCE_TYPE_BUFFER:
            {
                BufferRef buffer = std::static_pointer_cast< Buffer >( resource );
                buffer->Unmap( index );
            }
            return;
        case RESOURCE_TYPE_TEXTURE:
            {
                TextureRef texture = std::static_pointer_cast< Texture >( resource );
                const TextureDesc & desc = texture->GetDesc();

                VGL( glBindBuffer, GL_PIXEL_UNPACK_BUFFER, texture->GetBufferHandle() );
                {
                    GLboolean res = VGL( glUnmapBuffer, GL_PIXEL_UNPACK_BUFFER );
                    CMN_ASSERT( res != GL_FALSE ); CMN_UNUSED( res );

                    // update texture
                    if ( desc.usage == USAGE_DYNAMIC )
                    {
                        uint32_t mipWidth  = GetTextureMipWidth( desc.format, desc.width, index.mipSlice );
                        uint32_t mipHeight = GetTextureMipHeight( desc.format, desc.height, index.mipSlice );
                        size_t   mipOffset = GetTextureMipOffset( desc.format, desc.width, desc.height, index.mipSlice );

                        VGL( glBindTexture, GL_TEXTURE_2D, texture->GetHandle() );
                        VGL( glTexSubImage2D, GL_TEXTURE_2D, index.mipSlice, 0, 0,
                            mipWidth, mipHeight, GetOGLFormat( desc.format ),
                            GetOGLType( desc.format ), reinterpret_cast< void * >( mipOffset ) );
                        VGL( glBindTexture, GL_TEXTURE_2D, 0 );
                    }
                }
                VGL( glBindBuffer, GL_PIXEL_UNPACK_BUFFER, 0 );
            }
            return;
        default:
            CMN_FAIL();
        }
    }

    void Context::UpdateSubresource( IResourceIn resource, const SubResourceIndex & index, const SubResourceData & data )
    {
        CMN_ASSERT( resource->GetDesc().usage == USAGE_DEFAULT
            || resource->GetDesc().usage == USAGE_STAGING );

        switch ( resource->GetType() )
        {
        case RESOURCE_TYPE_BUFFER:
            {
                BufferRef buffer = std::static_pointer_cast< Buffer >( resource );
                buffer->UpdateSubresource( index, data );
            }
            break;
        default:
            CMN_FAIL(); // TODO not yet implemented
        }
    }

    void Context::CopyResource( IResourceIn dst, IResourceIn src )
    {
        CMN_ASSERT( dst );
        CMN_ASSERT( src );
        CMN_ASSERT( dst != src );   // cannot be the same resource
        CMN_ASSERT( dst->GetType() == src->GetType() ); // must be the same type

        switch ( dst->GetType() )
        {
        case RESOURCE_TYPE_BUFFER:
            {
                BufferRef srcBuffer = std::static_pointer_cast< Buffer >( src );
                BufferRef dstBuffer = std::static_pointer_cast< Buffer >( dst );

                CMN_ASSERT( srcBuffer->GetDesc().size == dstBuffer->GetDesc().size );

                VGL( glBindBuffer, GL_COPY_READ_BUFFER, srcBuffer->GetHandle() );
                VGL( glBindBuffer, GL_COPY_WRITE_BUFFER, dstBuffer->GetHandle() );

                VGL( glCopyBufferSubData, GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER,
                    0, 0, srcBuffer->GetDesc().size );

                VGL( glBindBuffer, GL_COPY_READ_BUFFER, 0 );
                VGL( glBindBuffer, GL_COPY_WRITE_BUFFER, 0 );
            }
            return;
        case RESOURCE_TYPE_TEXTURE:
            {
                TextureRef srcTexture = std::static_pointer_cast< Texture >( src );
                TextureRef dstTexture = std::static_pointer_cast< Texture >( dst );

                const TextureDesc & srcDesc = srcTexture->GetDesc();
                const TextureDesc & dstDesc = dstTexture->GetDesc();

                CMN_ASSERT( srcDesc.width == dstDesc.width );
                CMN_ASSERT( srcDesc.height == dstDesc.height );
                CMN_ASSERT( srcDesc.mipLevels == dstDesc.mipLevels );

                // none of the textures is staging
                if ( srcDesc.usage != USAGE_STAGING &&
                     dstDesc.usage != USAGE_STAGING )
                {
                    for ( uint32_t mipSlice = 0; mipSlice < srcDesc.mipLevels; ++ mipSlice )
                    {
                        VGL( glCopyImageSubData,
                            srcTexture->GetHandle(), GL_TEXTURE_2D, mipSlice, 0, 0, 0,
                            dstTexture->GetHandle(), GL_TEXTURE_2D, mipSlice, 0, 0, 0,
                            srcDesc.width, srcDesc.height, 1 );
                    }
                }
                // destination texture is staging
                else if ( srcDesc.usage != USAGE_STAGING &&
                    dstDesc.usage == USAGE_STAGING )
                {
                    VGL( glBindTexture, GL_TEXTURE_2D, srcTexture->GetHandle() );
                    VGL( glBindBuffer, GL_PIXEL_PACK_BUFFER, dstTexture->GetBufferHandle() );
                    for ( uint32_t mipSlice = 0; mipSlice < srcDesc.mipLevels; ++ mipSlice )
                    {
                        size_t mipOffset = GetTextureMipOffset( dstDesc.format,
                            dstDesc.width, dstDesc.height, mipSlice );
                        VGL( glGetTexImage, GL_TEXTURE_2D, mipSlice, GetOGLFormat( dstDesc.format ),
                            GetOGLType( dstDesc.format ), reinterpret_cast< void * >( mipOffset ) );
                    }
                    VGL( glBindTexture, GL_TEXTURE_2D, 0 );
                    VGL( glBindBuffer, GL_PIXEL_PACK_BUFFER, 0 );
                }
                // source texture is staging
                else if ( srcDesc.usage == USAGE_STAGING &&
                    dstDesc.usage != USAGE_STAGING )
                {
                    VGL( glBindBuffer, GL_PIXEL_UNPACK_BUFFER, srcTexture->GetBufferHandle() );
                    VGL( glBindTexture, GL_TEXTURE_2D, dstTexture->GetHandle() );
                    for ( uint32_t mipSlice = 0; mipSlice < srcDesc.mipLevels; ++ mipSlice )
                    {
                        size_t mipOffset = GetTextureMipOffset( srcDesc.format,
                            srcDesc.width, srcDesc.height, mipSlice );
                        VGL( glTexSubImage2D, GL_TEXTURE_2D, mipSlice, 0, 0, dstDesc.width, dstDesc.height,
                            GetOGLFormat( srcDesc.format ), GetOGLType( srcDesc.format ),
                            reinterpret_cast< void * >( mipOffset ) );
                    }
                    VGL( glBindBuffer, GL_PIXEL_UNPACK_BUFFER, 0 );
                    VGL( glBindTexture, GL_TEXTURE_2D, 0 );
                }
                // both textures are staging
                else if ( srcDesc.usage == USAGE_STAGING &&
                    dstDesc.usage == USAGE_STAGING )
                {
                    VGL( glBindBuffer, GL_COPY_READ_BUFFER, srcTexture->GetBufferHandle() );
                    VGL( glBindBuffer, GL_COPY_WRITE_BUFFER, dstTexture->GetBufferHandle() );

                    uint32_t size = GetTextureSize( srcDesc.format, srcDesc.width,
                        srcDesc.height, srcDesc.mipLevels );
                    VGL( glCopyBufferSubData, GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER,
                        0, 0, size );

                    VGL( glBindBuffer, GL_COPY_READ_BUFFER, 0 );
                    VGL( glBindBuffer, GL_COPY_WRITE_BUFFER, 0 );
                }
            }
            return;
        default:
            CMN_FAIL();
        };
    }

    void Context::Resolve( ITextureIn texture, const SubResourceIndex & index )
    {
        if ( texture )
        {
            CMN_ASSERT( mRenderTargets[ 0 ] );

            // setup shaders

                uint32_t shaderProgram = mDevice.lock()->GetResolveShaderProgram();
                VGL( glUseProgram, shaderProgram );

            // setup buffers

                VGL( glBindBuffer, GL_ARRAY_BUFFER, mScreenQuadBuffer );
                VGL( glVertexAttribPointer, 0, 2, GL_FLOAT, GL_TRUE,
                    2 * sizeof( float ), nullptr );
                VGL( glEnableVertexAttribArray, 0 );
                VGL( glBindBuffer, GL_ARRAY_BUFFER, 0 );

            // setup textures

                ConstTextureRef rtTex = std::static_pointer_cast< const Texture >(
                    mRenderTargets[ 0 ]->GetTexture() );
                uint64_t rtTexHandle = VGL( glGetTextureHandleARB, rtTex->GetHandle() );
                VGL( glMakeTextureHandleResidentARB, rtTexHandle );
                VGL( glUniformHandleui64ARB, 0, rtTexHandle );

            // setup framebuffer

                TextureRef texturePlat = std::static_pointer_cast< Texture >( texture );
                VGL( glBindFramebuffer, GL_DRAW_FRAMEBUFFER, mDrawFramebuffer );
                VGL( glFramebufferTexture, GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                    texturePlat->GetHandle(), index.mipSlice );
                VGL( glDrawBuffer, GL_COLOR_ATTACHMENT0 );

#ifdef CMN_DEBUG
                int32_t status  = VGL( glCheckFramebufferStatus, GL_DRAW_FRAMEBUFFER );
                CMN_ASSERT( status == GL_FRAMEBUFFER_COMPLETE );
#endif

            // draw

                VGL( glDrawArrays, GL_TRIANGLE_STRIP, 0, 4 );

            // release resources

                VGL( glMakeTextureHandleNonResidentARB, rtTexHandle );
                VGL( glUseProgram, 0 );

            // set dirty flags

                mDirtyFlags.renderTargets = true;
        }
    }

} // namespace GFW
