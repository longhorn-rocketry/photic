/**
 * Minimalist unit test API with zero dependencies.
 *
 * USAGE:
 *
 *   (1) A Test is a function which tests a related set of functions. Every
 *       Test begins with the TEST_DEFINE macro.
 *   (2) Tests employ Checks to verify observed behavior against expected
 *       behavior. Checks are performed with CHECK_XXX macros.
 *   (3) A Suite is a collection of Tests. A suite begins with the SUITE_START
 *       macro, calls all of its Test functions, and then concludes with the
 *       SUITE_END macro.
 *
 * NOTES:
 *
 *   (1) Depending on the compiler and STL functions available, the PRINTF
 *       macro in TestOutput.hpp may need to be changed.
 */

#ifndef TEST_MACROS_HPP
#define TEST_MACROS_HPP

#include "TestOutput.hpp"

/**
 * Global pass and fail counters for entire test suite.
 */
int gTestPasses = 0;
int gTestFails  = 0;

/**
 * Macro used to define a Test.
 *
 * @param   kName String literal test name.
 */
#define TEST_DEFINE(kName)                                                     \
    const char TEST_NAME[] = kName;                                            \
    PRINTF ("Running test %s...\n", TEST_NAME);

/**
 * Checks that two values are equal with the == operator.
 *
 * @param   kLhs LHS value.
 * @param   kRhs RHS value.
 */
#define CHECK_EQUAL(kLhs, kRhs)                                                \
    if (kLhs != kRhs)                                                          \
    {                                                                          \
        PRINTF ("CHECK_EQUAL failure in %s: %f != %f\n", TEST_NAME,            \
                (double) kLhs, (double) kRhs);                                 \
        gTestFails++;                                                          \
    }                                                                          \
    else                                                                       \
    {                                                                          \
        gTestPasses++;                                                         \
    }

/**
 * Checks that an expression is true.
 *
 * @param   kExpr Expression to evaluate.
 */
#define CHECK_TRUE(kExpr)                                                      \
    if (!(kExpr))                                                              \
    {                                                                          \
        PRINTF ("CHECK_TRUE failure in %s");                                   \
        gTestFails++;                                                          \
    }                                                                          \
    else                                                                       \
    {                                                                          \
        gTestPasses++;                                                         \
    }

/**
 * Starts a Suite.
 */
#define SUITE_START                                                            \
    PRINTF (">>> RUNNING ALL TESTS...\n")

/**
 * Ends a Suite by printing a test report.
 */
#define SUITE_END                                                              \
    PRINTF (">>> RAN %d CHECKS WITH %d SUCCESSES, %d FAILURES\n",              \
            gTestPasses + gTestFails, gTestPasses, gTestFails);

#endif