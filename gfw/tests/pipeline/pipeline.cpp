#include "gtest/gtest.h"

#include "gfw_pipeline/effect_builder.h"

using namespace GFW::Pipeline;

class GfwPipelineTests : public testing::Test
{
public:
    void SetUp()
    {

    }

    void TearDown()
    {

    }
};

TEST_F(GfwPipelineTests, Parse)
{
    EffectBuilderRef effectBuilder = new EffectBuilder();
    EffectBinaryRef  effectBinary  = effectBuilder->Build(TESTS_DIR "draw.fx");
}
