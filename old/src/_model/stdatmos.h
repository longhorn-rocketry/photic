/**
 * US Standard Atmosphere model.
 */
#ifndef PHOTIC_MODEL_STDATMOS_H
#define PHOTIC_MODEL_STDATMOS_H

#include <vector>

namespace photic
{

/**
 * Atmospheric conditions at an altitude.
 */
typedef struct Atmos
{
    float altitude;          // m
    float temperature;       // C
    float gravity;           // m/s^2
    float pressure;          // N/m^2m
    float air_density;       // kg/m^3
    float dynamic_viscosity; // N*s/m^2
} Atmos_t;

/**
 * Data points in the US Standard Atmosphere ordered by increasing altitude.
 */
const Atmos_t STANDARD_ATMOSPHERE[] =
{
    { 0000,  15.0000, 9.8070, 10.1300, 1.2250, 1.7890},
    { 1000,   8.5000, 9.8040,  8.9880, 1.1120, 1.7580},
    { 2000,   2.0000, 9.8010,  7.9500, 1.0070, 1.7260},
    { 3000,  -4.4900, 9.7970,  7.0120, 0.9093, 1.6940},
    { 4000, -10.9800, 9.7940,  6.1660, 0.8194, 1.6610},
    { 5000, -17.4700, 9.7910,  5.4050, 0.7364, 1.6280},
    { 6000, -23.9600, 9.7880,  4.7220, 0.6601, 1.5950},
    { 7000, -30.4500, 9.7850,  4.1110, 0.5900, 1.5610},
    { 8000, -36.9400, 9.7820,  3.5650, 0.5258, 1.5270},
    { 9000, -43.4200, 9.7790,  3.0800, 0.4671, 1.4930},
    {10000, -49.9000, 9.7760,  2.6500, 0.4135, 1.4580}
};

/**
 * Number of data points in STANDARD_ATMOSPHERE.
 */
const std::size_t STDATMOS_SIZE = 11;

/**
 * Gets the conditions at a particular altitude according to a linear
 * interpolation of the US Standard Atmosphere. If the altitude is out of range,
 * either the first or last entries in the table are returned.
 *
 * @param   altitude Altitude MSL (m).
 *
 * @ret     Atmospheric conditions at altitude.
 */
Atmos_t stdatmos(float altitude);

} // namespace photic

#endif
