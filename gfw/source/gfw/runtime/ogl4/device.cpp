#include "cmn/scope_exit.h"
#include "cmn/trace.h"
#include "gfw/device_child.h"
#include "gfw/resource.h"
#include "gfw/runtime/ogl4/buffer.h"
#include "gfw/runtime/ogl4/context.h"
#include "gfw/runtime/ogl4/device.h"
#include "gfw/runtime/ogl4/functions.h"
#include "gfw/runtime/ogl4/input_layout.h"
#include "gfw/runtime/ogl4/render_target.h"
#include "gfw/runtime/ogl4/shader.h"
#include "gfw/runtime/ogl4/shader_reflect.h"
#include "gfw/runtime/ogl4/shader_utils.h"
#include "gfw/runtime/ogl4/swap_chain.h"
#include "gfw/runtime/ogl4/texture.h"

#define AUTO_LOCK_CONTEXT \
    /* Lock the mutex */ \
    std::lock_guard< std::mutex > __autoLock_mutex( mMutex ); \
    /* Save current context */ \
    NativeContext * __autoLock_prevContext = mSwapChain->GetCurrentContext(); \
    /* Set new context */ \
    mSwapChain->MakeCurrent( mNativeContext.get() ); \
    /* Restore context on leaving scope */ \
    auto __autoLock_scopeExit = Cmn::MakeScopeExit( \
        [ this, __autoLock_prevContext ] () { \
            mSwapChain->MakeCurrent( __autoLock_prevContext ); \
        } )

// Allows destruct a child in a specific native context
#define DEVICE_CHILD_DELETER( typeName ) \
    [ this ] ( typeName * child ) { \
        mSwapChain->MakeCurrent( mNativeContext.get() ); \
            delete child; \
        mSwapChain->MakeCurrent( nullptr ); \
    }

namespace GFW {

    static const char sResolveVertexShader[] = {
        R"code(

            #version 440 core

            in vec2 position;
            out vec2 texcoord;

            void main()
            {
                gl_Position = vec4( position, 0.0f, 1.0f );
                texcoord = position * vec2( 0.5f, -0.5f )
                    + vec2( 0.5f, 0.5f );
            }

        )code"
    };

    static const char sResolveFragmentShader[] = {
        R"code(

            #version 440 core

            in vec2 texcoord;
            out vec4 color;

            layout( location = 0 ) uniform sampler2D textureSampler;

            void main()
            {
                color = texture( textureSampler, texcoord );
            }

        )code"
    };

    CMN_THREAD_LOCAL IContext * Device::mCurrentContext = nullptr;

    Device::Device( const DeviceParams & params, ISwapChainIn swapChain )
        : mParams( params )
        , mSwapChain( std::static_pointer_cast< SwapChain >( swapChain ) )
        , mNativeContext( mSwapChain->GetDefaultNativeContext() )
        , mResolveShaderProgram( 0 )
    {
        AUTO_LOCK_CONTEXT;

        uint32_t vertexShader = CompileShader( SHADER_STAGE_VERTEX, sResolveVertexShader );
        uint32_t fragmentShader = CompileShader( SHADER_STAGE_PIXEL, sResolveFragmentShader );

        mResolveShaderProgram = VGL( glCreateProgram );
        VGL( glAttachShader, mResolveShaderProgram, vertexShader );
        VGL( glAttachShader, mResolveShaderProgram, fragmentShader );
        LinkProgram( mResolveShaderProgram );

#if defined( CMN_DEBUG )
        const uint8_t * extensions = glGetString(GL_EXTENSIONS);
        CMN_UNUSED( extensions );
#endif
    }

    Device::~Device()
    {
        if ( mResolveShaderProgram )
        {
            VGL( glDeleteProgram, mResolveShaderProgram );
        }
    }

    void Device::InitializeChildren()
    {
        mDefaultContext = CreateContext();
    }

    IContextRef Device::CreateContext()
    {
        // AUTO_LOCK_CONTEXT is not necessary because the context is constucted in its own native context
        std::lock_guard< std::mutex > lock( mMutex );

        NativeContextRef nativeContext = mSwapChain->CreateContext();
        mSwapChain->ShareLists( mNativeContext.get(), nativeContext.get() );

        // Constuct the context in its own native context
        mSwapChain->MakeCurrent( nativeContext.get() );
        auto onScopeExit = Cmn::MakeScopeExit( [ this ] () {
                mSwapChain->MakeCurrent( nullptr );
            } );

        return ContextRef( new Context( nativeContext, shared_from_this() ),
            // Make sure that the context will be deleted in its own native context
            [ this, nativeContext ] ( Context * ctx ) {
                mSwapChain->MakeCurrent( nativeContext.get() );
                    delete ctx;
                mSwapChain->MakeCurrent( nullptr );
            } );
    }

    IShaderRef Device::CreateShader( ShaderStage stage, const void * shaderBinary )
    {
        AUTO_LOCK_CONTEXT;
        return ShaderRef( new Shader( shaderBinary, stage, shared_from_this() ),
            DEVICE_CHILD_DELETER( Shader ) );
    }

    IInputLayoutRef Device::CreateInputLayout( uint32_t attrCnt, VertexAttribute attrs[], ConstIShaderIn shader )
    {
        AUTO_LOCK_CONTEXT;
        CMN_ASSERT( shader );
        return InputLayoutRef( new InputLayout( attrCnt, attrs, shader, shared_from_this() ),
            DEVICE_CHILD_DELETER( InputLayout ) );
    }

    IBufferRef Device::CreateBuffer( const BufferDesc & desc, const SubResourceData * initialData /* = nullptr */ )
    {
        CMN_ASSERT( desc.size != 0 );
        CMN_ASSERT( ( desc.cpuAccessFlags & CPU_ACCESS_FLAG_READ ) ? // only staging resource can be read
            ( desc.usage == USAGE_STAGING ) : 1 );
        CMN_ASSERT( ( desc.cpuAccessFlags & CPU_ACCESS_FLAG_WRITE ) ? // only dynamice or staging resource can be written
            ( desc.usage == USAGE_STAGING || desc.usage == USAGE_DYNAMIC ) : 1 );

        AUTO_LOCK_CONTEXT;
        return BufferRef( new Buffer( desc, initialData, shared_from_this() ),
            DEVICE_CHILD_DELETER( Buffer ) );
    }

    ITextureRef Device::CreateTexture( const TextureDesc & desc, const SubResourceData * initialData /* = nullptr */ )
    {
        CMN_ASSERT( desc.width != 0 );
        CMN_ASSERT( desc.height != 0 );
        CMN_ASSERT( desc.format != FORMAT_UNKNOWN );
        CMN_ASSERT( ( desc.cpuAccessFlags & CPU_ACCESS_FLAG_READ ) ? // only staging resource can be read
            ( desc.usage == USAGE_STAGING ) : 1 );
        CMN_ASSERT( ( desc.cpuAccessFlags & CPU_ACCESS_FLAG_WRITE ) ? // only dynamice or staging resource can be written
            ( desc.usage == USAGE_STAGING || desc.usage == USAGE_DYNAMIC ) : 1 );

        AUTO_LOCK_CONTEXT;
        return TextureRef( new Texture( desc, initialData, shared_from_this() ),
            DEVICE_CHILD_DELETER( Texture ) );
    }

    IRenderTargetRef Device::CreateRenderTarget( ConstITextureIn texture, const RenderTargetDesc & desc )
    {
        AUTO_LOCK_CONTEXT;
        return RenderTargetRef( new RenderTarget(
                std::static_pointer_cast< const Texture >( texture ),
                desc, shared_from_this() ),
            DEVICE_CHILD_DELETER( RenderTarget ) );
    }

    IContextRef Device::GetCurrentContext() const
    {
        return static_cast< Context * >( mCurrentContext )->shared_from_this();
    }

    void Device::LockContext( IContextIn context )
    {
        // No need to lock the mutex, because reference to the current context is hold in a thread local storage
        CMN_ASSERT( context );
        CMN_ASSERT( mCurrentContext == nullptr ); // The same thread cannot lock multiple contexts
        mSwapChain->MakeCurrent( std::static_pointer_cast< Context >( context )->GetNativeContext().get() );
        mCurrentContext = context.get();
    }

    void Device::UnlockContext( IContextIn context )
    {
        // No need to lock the mutex, see LockContext()
        CMN_ASSERT( context );
        CMN_ASSERT( mCurrentContext == context.get() );
        mSwapChain->MakeCurrent( nullptr );
        mCurrentContext = nullptr;
    }

} // namespace GFW
