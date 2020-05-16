#include <limits>
#include <math.h>
#include <vector>

#include "float16.h"

#include <CppUTest/TestHarness.h>

using namespace photic;

TEST_GROUP(Float16Test)
{

};

/**
 * float32-16 conversion works correctly.
 */
TEST(Float16Test, CompressDecompress)
{
    // Ensure not too much error is lost between compression and decompression.
    const std::vector<float> vals =
            {0.223, 0.565, 3.1112, 657.43, -54.23, 1765.223};

    for (std::size_t i = 0; i < vals.size(); i++)
    {
        float16 fs = f32to16(vals[i]);
        float fd = f16to32(fs);
        float err = fabs(fd - vals[i]);

        // Error as large as 1.0 is to be expected given the largest test input.
        CHECK_TRUE(err < 1.0);
    }

    // NaN is unchanged by compression.
    float f = sqrt(-1.0);
    CHECK_TRUE(isnan(f));
    float f16 = f32to16(f);
    float fd = f16to32(f16);
    CHECK_TRUE(isnan(fd));

    // Infinity is unchanged by compression.
    f = std::numeric_limits<float>::infinity();
    CHECK_TRUE(!isfinite(f));
    f16 = f32to16(f);
    fd = f16to32(f16);
    CHECK_TRUE(!isfinite(fd));
}
