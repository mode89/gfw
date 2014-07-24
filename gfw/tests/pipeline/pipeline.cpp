#pragma warning( push )
#pragma warning( disable : 4625 4626 )
#include "gtest/gtest.h"
#pragma warning( pop )

#include "gfw/pipeline/common/effect_builder.h"
#include "gfw/shared/effect.h"
#include "gfw/shared/shader.h"
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

TEST_F(GfwPipelineTests, Draw)
{
    EffectBinary  effectBinary;
    EffectBuilder effectBuilder;
    effectBuilder.Build( effectBinary, TESTS_DATA_DIR "draw.fx" );
}
