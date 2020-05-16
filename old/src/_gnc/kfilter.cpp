#include "gnc/kfilter.h"

namespace photic
{

KalmanFilter::KalmanFilter()
{
    m_A = matrix(3, 3,
        1, 0, 0,
        0, 1, 0,
        0, 0, 1
    );
    m_Q = matrix(3, 3,
        0, 0, 0,
        0, 0, 0,
        0, 0, 0
    );
    m_H = matrix(2, 3,
        1, 0, 0,
        0, 0, 1
    );
    m_R = matrix(2, 2,
        0, 0,
        0, 0
    );
    m_P = matrix(3, 3,
        1, 0, 0,
        0, 1, 0,
        0, 0, 1
    );
}

void KalmanFilter::set_delta_t(float k_dt)
{
    m_A[0][1] = k_dt;
    m_A[0][2] = 0.5 * k_dt * k_dt;
    m_A[1][2] = k_dt;
}

void KalmanFilter::set_sensor_variance(float k_altimeter_variance,
                                       float k_accelerometer_variance)
{
    m_R[0][0] = k_altimeter_variance;
    m_R[1][1] = k_accelerometer_variance;
}

void KalmanFilter::set_initial_estimate(float k_s, float k_v, float k_a)
{
    m_estimate = matrix(3, 1, k_s, k_v, k_a);
}

void KalmanFilter::compute_kg(uint32_t k_iterations)
{
    m_P = matrix(3, 3,
        1, 0, 0,
        0, 1, 0,
        0, 0, 1
    );
    for (uint32_t i = 0; i < k_iterations; i++)
    {
        this->compute_kg();
    }
}

void KalmanFilter::compute_kg()
{
    m_K = m_P * m_H.t() * (m_H * m_P * m_H.t() + m_R).inv();
    matrix ident(3, 3,
        1, 0, 0,
        0, 1, 0,
        0, 0, 1
    );
    m_P = (ident - m_K * m_H) * m_P;
    m_P = m_A * m_P * m_A.t() + m_Q;
}

void KalmanFilter::set_kg(matrix k_kg)
{
    m_K = k_kg;
}

matrix KalmanFilter::filter(float k_s, float k_a)
{
    matrix observation(2, 1, k_s, k_a);
    matrix estimate_new = m_A * m_estimate;
    matrix estimate_new_f = estimate_new + m_K *
                          (observation - m_H * estimate_new);
    m_estimate = estimate_new_f;

    return m_estimate;
}

} // namespace photic
