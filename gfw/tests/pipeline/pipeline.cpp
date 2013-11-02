#include "gtest/gtest.h"

#include "gfw/pipeline/common/effect_builder.h"

using namespace Common;
using namespace GFW;

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
    EffectBuilderRef effectBuilder = new EffectBuilder;
    EffectBinaryRef  effectBinary  = effectBuilder->Build(TESTS_DIR "draw.fx" );
}
