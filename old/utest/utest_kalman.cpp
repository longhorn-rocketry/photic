#include <ctime>
#include <math.h>
#include <random>

#include "kfilter.h"
#include "testutil.h"

#include <CppUTest/TestHarness.h>

#define POS_VARIANCE 15.45
#define ACC_VARIANCE 1.8

using namespace photic;

TEST_GROUP(KalmanFilterTest)
{

};

/**
 * Kalman filter improves accuracy when tuned correctly.
 *
 * Note: because this test is based on chance, it may fail. As long as it fails
 * very, very rarely, the Kalman filter is working satisfactorily.
 */
TEST(KalmanFilterTest, AccuracyIncrease)
{
    // Current timestep, timestep size, and duration of simulation.
    float t = 0;
    float t_step = 0.1;
    float duration = 100;

    // Error distributions for position and acceleration observations.
    std::mt19937 generator(std::time(0));
    std::normal_distribution<float> pos_err_distr(0, sqrt(POS_VARIANCE));
    std::normal_distribution<float> acc_err_distr(0, sqrt(ACC_VARIANCE));

    // Kalman filter configured to match the above distributions.
    KalmanFilter kf;
    kf.set_delta_t(t_step);
    kf.set_sensor_variance(POS_VARIANCE, ACC_VARIANCE);
    kf.set_initial_estimate(0, 0, 0);
    kf.compute_kg(100);

    // True state of the system.
    matrix state_true = matrix(3, 1, 0, 0, 0);
    // State estimated by pure dead reckoning.
    matrix state_deadreck = state_true;
    // State estimated by the Kalman filter.
    matrix state_filtered = state_true;

    while (t < duration)
    {
        // Extrapolate true state.
        state_true[2][0] = 9.81;
        state_true[1][0] += state_true[2][0] * t_step;
        state_true[0][0] += state_true[1][0] * t_step;

        // Observe true state by applying errors.
        float acc_observed = state_true[2][0] + acc_err_distr(generator);
        float pos_observed = state_true[0][0] + pos_err_distr(generator);

        // Update dead-reckoned estimate.
        state_deadreck[2][0] = acc_observed;
        state_deadreck[1][0] += state_deadreck[2][0] * t_step;
        state_deadreck[0][0] += state_deadreck[1][0] * t_step;

        // Update filtered estimate.
        state_filtered = kf.filter(pos_observed, acc_observed);

        // Advance time.
        t += t_step;
    }

    matrix err_deadreck = state_true - state_deadreck;
    matrix err_filtered = state_true - state_filtered;

    // Print results.
    // printf("--- KalmanFilterTest::AccuracyIncrease ---\n");
    // printf("True state:          [%10.2f, %10.2f, %10.2f]\n",
    //        state_true[0][0], state_true[1][0], state_true[2][0]);
    // printf("Dead-reckoned state: [%10.2f, %10.2f, %10.2f]\n",
    //        state_deadreck[0][0], state_deadreck[1][0], state_deadreck[2][0]);
    // printf("Filtered state       [%10.2f, %10.2f, %10.2f]\n",
    //        state_filtered[0][0], state_filtered[1][0], state_filtered[2][0]);
    //
    //
    // printf("Dead reckon error:   [%10.2f, %10.2f, %10.2f]\n",
    //        err_deadreck[0][0], err_deadreck[1][0], err_deadreck[2][0]);
    // printf("Filter error:        [%10.2f, %10.2f, %10.2f]\n",
    //        err_filtered[0][0], err_filtered[1][0], err_filtered[2][0]);

    // Filter position error should be less than dead reckon position error.
    CHECK_TRUE(fabs(err_filtered[0][0]) < fabs(err_deadreck[0][0]));
}
