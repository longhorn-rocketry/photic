#include <CppUTest/TestHarness.h>

#include "metronome.h"
#include "testutil.h"

using namespace photic;

TEST_GROUP(MetronomeTest)
{

};

TEST(MetronomeTest, Poll)
{
    Metronome m(100);

    CHECK_APPROX(0.01, m.period());
    CHECK_EQUAL(-1, m.dt());
    CHECK_TRUE(m.poll(0));

    m.reset();

    CHECK_TRUE(m.poll(-10));
    CHECK_TRUE(!m.poll(-9.995));
    CHECK_TRUE(m.poll(-9.99));
    CHECK_APPROX(0.01, m.dt());
    CHECK_TRUE(m.poll(-9.59));
    CHECK_APPROX(0.4, m.dt());

    m.reset();

    CHECK_EQUAL(-1, m.dt());
}
