#include "gtest/gtest.h"
#include "gfw/pipeline/common/effect_builder.h"
#include "gfw/shared/effect.h"
#include "gfw/shared/shader.h"
#include "gfw/shared/technique.h"
#include "gfw/shared/pass.h"

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

TEST_F(GfwPipelineTests, Draw)
{
    EffectBinary  effectBinary;
    EffectBuilder effectBuilder;
    effectBuilder.Build( effectBinary, TESTS_DATA_DIR "draw.fx" );
}
