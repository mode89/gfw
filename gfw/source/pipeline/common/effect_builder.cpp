#include "cmn/trace.h"
#include "gfw/pipeline/common/effect_builder.h"
#include "gfw/pipeline/common/effect_builder_exception.h"
#include "gfw/pipeline/common/parse_tree.h"
#include "gfw/pipeline/common/symbol_table.h"
#include "gfw/pipeline/shader_builder.h"
#include "gfw/shared/effect.h"
#include "gfw/shared/pass.h"
#include "gfw/shared/shader.h"
#include "gfw/shared/technique.h"

namespace GFW {

    EffectBuilder::EffectBuilder()
    {

    }

    void EffectBuilder::Build( EffectBinary & effectBinary, const boost::filesystem::path & filePath )
    {
        // TODO Replace with stack allocation
        std::shared_ptr< const ParseTree > tree = CreateParseTree( filePath );

        // Collect symbols
        SymbolTable symbolTable( *tree );

        // Construct shader builder
        std::shared_ptr< ShaderBuilder > shaderBuilder = std::make_shared<ShaderBuilder>( *tree, symbolTable );
        mShaderBuilder = shaderBuilder;

        // Allocate a shader table
        std::shared_ptr< ShaderTable > shaderTable = std::make_shared<ShaderTable>();
        mShaderTable = shaderTable;

        mEffectBinary = &effectBinary;

        // Process techniques
        tree->TraverseDFS( *this, &EffectBuilder::ProcessTechniques );
    }

    bool EffectBuilder::ProcessTechniques( const ParseTree & tree )
    {
        if ( tree.GetTokenType() == TOKEN_EXTERNAL_DECLARATION )
        {
            const ParseTree & child = tree.GetChild();
            if ( child.GetTokenType() == TOKEN_TECHNIQUE_DEFINITION )
            {
                const std::string & name = child.GetChild().GetText();

                // Allocate a technique binary and add it to the effect
                auto & techniques = mEffectBinary->mTechniques;
                techniques.emplace_back();
                TechniqueBinary & techniqueBinary = techniques.back();

                techniqueBinary.mName = name;

                child.TraverseDFS( *this, &EffectBuilder::ProcessPasses );
            }
            return false;
        }
        return true;
    }

    bool EffectBuilder::ProcessPasses( const ParseTree & tree )
    {
        if ( tree.GetTokenType() == TOKEN_PASS_DEFINITION )
        {
            const std::string & name = tree.GetChild().GetText();

            // Allocate a pass binary and add it to the current technique
            auto & passes = mEffectBinary->mTechniques.back().mPasses;
            passes.emplace_back();
            PassBinary & passBinary = passes.back();

            passBinary.mName = name;

            tree.TraverseDFS( *this, &EffectBuilder::ProcessShaders );

            return false;
        }
        return true;
    }

    bool EffectBuilder::ProcessShaders( const ParseTree & tree )
    {
        if ( tree.GetTokenType() == TOKEN_SET_SHADER )
        {
            const ParseTree & compiledShader = tree.GetChild( 1 );

            if ( compiledShader.GetTokenType() == TOKEN_COMPILE_SHADER )
            {
                const std::string & name = compiledShader.GetChild( 1 ).GetText();
                const std::string & profile = compiledShader.GetChild( 0 ).GetText();

                // Allocate a shader binary
                mEffectBinary->mShaderTable.emplace_back();
                ShaderBinary & shaderBinary = mEffectBinary->mShaderTable.back();

                // Compile the shader
                try {
                    mShaderBuilder.lock()->Build( shaderBinary, name, profile );
                } catch ( std::exception  e ) {
                    TRACE_ERR( e.what() );
                    TRACE_THROW( EffectBuilderException::ShaderBuilderError( name.c_str() ) );
                } catch ( ... ) {
                    TRACE_ERR( "Unrocegnized ShaderBuilder exception." );
                }

                PassBinary & pass = mEffectBinary->mTechniques.back().mPasses.back();

                const ParseTree & shaderType = tree.GetChild( 0 );
                switch ( shaderType.GetTokenType() )
                {
                case TOKEN_SET_VERTEX_SHADER:
                    pass.mShaders[ ShaderStage::VERTEX ] = &shaderBinary;
                    break;
                case TOKEN_SET_PIXEL_SHADER:
                    pass.mShaders[ ShaderStage::PIXEL ] = &shaderBinary;
                    break;
                default:
                    CMN_FAIL();
                    break;
                }
            }
            else
            {
                CMN_FAIL();
            }

            return false;
        }
        return true;
    }

} // namespace GFW
