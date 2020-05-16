#include "stdatmos.h"
#include "testutil.h"

#include <CppUTest/TestHarness.h>
#include <iostream>

using namespace photic;

TEST_GROUP(StdatmosTest)
{

};

/**
 * Atmospheric condition lookups are correct.
 */
TEST(StdatmosTest, Lookup)
{
    Atmos_t atmos;

    atmos = stdatmos(0);

    CHECK_APPROX(atmos.temperature, 15);

    atmos = stdatmos(-5);

    CHECK_APPROX(atmos.temperature, 15);

    atmos = stdatmos(4000);

    CHECK_APPROX(atmos.temperature, -10.98);

    atmos = stdatmos(7300);
    Atmos_t a0 = stdatmos(7000);
    Atmos_t a1 = stdatmos(8000);

    CHECK_TRUE(atmos.temperature < a0.temperature);
    CHECK_TRUE(atmos.temperature > a1.temperature);

    atmos = stdatmos(11000);

    CHECK_APPROX(atmos.temperature, -49.9);

}
