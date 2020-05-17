/**
 * Tests for Metronome.
 */

#ifndef TEST_METRONOME_HPP
#define TEST_METRONOME_HPP

#include "Metronome.hpp"
#include "TestMacros.hpp"

namespace TestMetronome
{

/**
 * Tests that Metronomes are polled and compute dts correctly.
 */
void test ()
{
    TEST_DEFINE ("MetronomePolling");

    Metronome met (0.01);

    // Check that dt without polling is TIME_NONE constant.
    CHECK_EQUAL (met.getDt (), Metronome::TIME_NONE);

    // Poll Metronome at inconsistent intervals and check that it ticks
    // correctly and that dts are correct.
    CHECK_TRUE  (met.poll (0));                       // First poll is a tick.
    CHECK_EQUAL (met.getDt (), Metronome::TIME_NONE); // No dt w/ just 1 poll.

    CHECK_TRUE (!met.poll (0.005));

    // Tick 1.
    CHECK_TRUE   (met.poll (0.01));
    CHECK_APPROX (met.getDt (), 0.01, 1e-6);

    CHECK_TRUE (!met.poll (0.01));
    CHECK_TRUE (!met.poll (0.019));

    // Tick 2.
    CHECK_TRUE   (met.poll (0.02));
    CHECK_APPROX (met.getDt (), 0.01, 1e-6);

    // Tick 3.
    CHECK_TRUE   (met.poll (5));
    CHECK_APPROX (met.getDt (), 4.98, 1e-6);

    CHECK_TRUE (!met.poll (5.005));

    // Tick 4.
    CHECK_TRUE   (met.poll (5.01));
    CHECK_APPROX (met.getDt (), 0.01, 1e-6);
}

} // namespace TestMetronome

#endif