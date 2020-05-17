/**
 * Tests for History.
 */

#ifndef TEST_HISTORY_HPP
#define TEST_HISTORY_HPP

#include "History.hpp"
#include "TestMacros.hpp"

using namespace Photic;

namespace TestHistory
{

/**
 * Tests that history mean and stdev are computed correctly.
 */
void testHistoryStats ()
{
    TEST_DEFINE ("HistoryStats");

    // Check that empty history and 1-element histories compute correct stats.
    History<5> hist;
    CHECK_TRUE (isnan (hist.getMean ())); // Mean of 0 elements is undefined.
    CHECK_EQUAL (hist.getStdev (), 0);

    hist.add (2);
    CHECK_EQUAL (hist.getMean (), 2);
    CHECK_EQUAL (hist.getStdev (), 0);

    // Overflow the history so it is forced to throw out old data.
    hist.add (12);
    hist.add (17);
    hist.add (4);
    hist.add (7);

    // Check that the most recent data remains in history.
    CHECK_APPROX (hist.getStdev (), 5.4626, 0.0001);
    CHECK_APPROX (hist.getMean (), 8.4, 0.0001);

    // Add something new and verify that new stats are computed.
    hist.add (8);
    CHECK_APPROX (hist.getStdev (), 4.4988, 0.0001);
    CHECK_APPROX (hist.getMean (), 9.6, 0.0001);
}

/**
 * Tests history capacitated behavior and clear operation.
 */
void testHistoryCapAndClear ()
{
    TEST_DEFINE ("HistoryCapAndClear");

    // Add data to history one at a time, checking capacity and finally mean.
    History<3> hist;
    CHECK_TRUE (!hist.atCapacity ());
    hist.add (1);
    CHECK_TRUE (!hist.atCapacity ());
    hist.add (1);
    CHECK_TRUE (!hist.atCapacity ());
    hist.add (1);
    CHECK_TRUE (hist.atCapacity ());
    CHECK_EQUAL (hist.getMean (), 1);

    // Clear the history and try again with different data.
    hist.clear ();
    CHECK_TRUE (!hist.atCapacity ());
    hist.add (2);
    CHECK_TRUE (!hist.atCapacity ());
    hist.add (2);
    CHECK_EQUAL (hist.getMean (), 2); // If prev clear fails, so will this.
    CHECK_TRUE (!hist.atCapacity ());
    hist.add (2);
    CHECK_TRUE (hist.atCapacity ());
    CHECK_EQUAL (hist.getMean (), 2);
}

/**
 * Entry point for history tests.
 */
void test ()
{
    testHistoryStats ();
    testHistoryCapAndClear ();
}

} // namespace TestHistory

#endif