#include "kalman_filter.h"

using namespace photonic;

KalmanFilter::KalmanFilter() {
  A = matrix(3, 3,
      1, 0, 0,
      0, 1, 0,
      0, 0, 1);
  Q = matrix(3, 3,
      0, 0, 0,
      0, 0, 0,
      0, 0, 0);
  H = matrix(2, 3,
      1, 0, 0,
      0, 0, 1);
  R = matrix(2, 2,
      0, 0,
      0, 0);
  P = matrix(3, 3,
      1, 0, 0,
      0, 1, 0,
      0, 0, 1);
}

void KalmanFilter::set_delta_t(float dt) {
  A[0][1] = dt;
  A[0][2] = 0.5 * dt * dt;
  A[1][2] = dt;
}

void KalmanFilter::set_sensor_variance(float altimeter_variance,
    float accelerometer_variance) {
  R[0][0] = altimeter_variance;
  R[1][1] = accelerometer_variance;
}

void KalmanFilter::set_initial_estimate(float s, float v, float a) {
  estimate = matrix(3, 1, s, v, a);
}

void KalmanFilter::compute_kg(int iterations) {
  P = matrix(3, 3,
      1, 0, 0,
      0, 1, 0,
      0, 0, 1);
  for (int i = 0; i < iterations; i++)
    compute_kg();
}

void KalmanFilter::compute_kg() {
  K = P * H.t() * (H * P * H.t() + R).inv2x2();
  matrix ident(3, 3,
      1, 0, 0,
      0, 1, 0,
      0, 0, 1);
  P = (ident - K * H) * P;
  P = A * P * A.t() + Q;
}

void KalmanFilter::set_kg(matrix kg) {
  K = kg;
}

matrix KalmanFilter::filter(float s, float a) {
  matrix observation(2, 1, s, a);
  matrix estimate_new = A * estimate;
  matrix estimate_new_f = estimate_new + K * (observation - H * estimate_new);
  estimate = estimate_new_f;
  return estimate;
}
