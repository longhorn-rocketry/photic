#include "history.h"
#include "math.h"
#include "testutil.h"

#include <CppUTest/TestHarness.h>

using namespace photic;

TEST_GROUP(HistoryTest)
{

};

/**
 * Histories are expanded, accessed, and mutated properly.
 */
TEST(HistoryTest, AddAccessMutate)
{
    history<float> h(10);

    CHECK_EQUAL(10, h.capacity());
    CHECK_EQUAL(0, h.size());

    h.add(2);
    h.add(3);
    h.add(4);

    CHECK_EQUAL(10, h.capacity());
    CHECK_EQUAL(3, h.size());

    CHECK_EQUAL(2, h[0]);
    CHECK_EQUAL(3, h[1]);
    CHECK_EQUAL(4, h[2]);

    h[1] = 6;

    CHECK_EQUAL(2, h[0]);
    CHECK_EQUAL(6, h[1]);
    CHECK_EQUAL(4, h[2]);
}

/**
 * Histories perform correct statistical analysis. Capacity checking and
 * clearing work as intended.
 */
TEST(HistoryTest, Stats)
{
    history<float> h(5);

    CHECK_TRUE(isnan(h.mean()));
    CHECK_EQUAL(0, h.stdev());

    h.add(2);
    h.add(12);
    h.add(17);
    h.add(4);
    h.add(7);

    CHECK_TRUE(h.at_capacity());
    CHECK_APPROX(5.4626001134991, h.stdev());
    CHECK_APPROX(8.4, h.mean());

    h.add(8);

    CHECK_TRUE(h.at_capacity());
    CHECK_APPROX(4.4988887516808, h.stdev());
    CHECK_APPROX(9.6, h.mean());

    h.clear();
    h.add(7);

    CHECK_TRUE(!h.at_capacity());
    CHECK_APPROX(0, h.stdev());
    CHECK_APPROX(7, h.mean());
}
