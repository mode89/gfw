#include "cmn/platform.h"

CMN_WARNING_PUSH
CMN_WARNING_DISABLE_MSVC( 4625 4626 )
#include "gtest/gtest.h"
CMN_WARNING_POP

#include "gfw/pipeline/fx/common/effect_builder.h"
#include "gfw/pipeline/scene/common/scene_builder.h"
#include "gfw/shared/effect.h"
#include "gfw/shared/shader.h"
#include "gfw/shared/scene_binary.h"
#include "gfw/shared/technique.h"
#include "gfw/shared/pass.h"

using namespace GFW;

class GfwPipelineTests : public testing::Test
{
public:
    GfwPipelineTests()
    {
    }

    void SetUp()
    {
    }

    void TearDown()
    {
    }

    GfwPipelineTests( const GfwPipelineTests & );
    GfwPipelineTests & operator= ( const GfwPipelineTests & );
};

TEST_F( GfwPipelineTests, Draw )
{
    EffectBinary  effectBinary;
    EffectBuilder effectBuilder;
    effectBuilder.Build( effectBinary, TESTS_DATA_DIR "draw.fx" );
}

TEST_F( GfwPipelineTests, ColladaQuad )
{
    SceneBinary sceneBinary;
    ISceneBuilderRef sceneBuilder = CreateSceneBuilderCollada();
    sceneBuilder->Build( sceneBinary, TESTS_DATA_DIR "quad.dae" );
}

TEST_F( GfwPipelineTests, ColladaSphere )
{
    SceneBinary sceneBinary;
    ISceneBuilderRef sceneBuilder = CreateSceneBuilderCollada();
    sceneBuilder->Build( sceneBinary, TESTS_DATA_DIR "sphere.dae" );
}
