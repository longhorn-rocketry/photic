#include <stddef.h>

#include "math/rocketmath.h"
#include "model/stdatmos.h"
#include "util/structutil.h"

namespace photic
{

Atmos_t stdatmos(float alt)
{
    // Search lookup table for index of altitude conditions.
    Atmos_t conds;
    conds.altitude = -1;
    std::size_t idx = structarr_binsearch<float, Atmos_t>(
            STANDARD_ATMOSPHERE, STDATMOS_SIZE, alt,
            offsetof(Atmos_t, altitude), conds);

    // If an exact match in the table was found, return that.
    if (conds.altitude != -1)
    {
        return conds;
    }

    // If altitude is beyond the table, return the last entry.
    if (idx == STDATMOS_SIZE)
    {
        return STANDARD_ATMOSPHERE[idx - 1];
    }
    // If altitude is below the table, return the last entry.
    else if (idx == 0)
    {
        return STANDARD_ATMOSPHERE[idx];
    }

    // Look up conditions on either side of index where alt would go.
    Atmos_t lower = STANDARD_ATMOSPHERE[idx - 1];
    Atmos_t upper = STANDARD_ATMOSPHERE[idx];
    Atmos_t ret;

    // Interpolate new conditions.
    float p = (alt - lower.altitude) / (upper.altitude - lower.altitude);
    ret.altitude = alt;
    ret.temperature = lerp<float>(lower.temperature, upper.temperature, p);
    ret.gravity = lerp<float>(lower.gravity, upper.gravity, p);
    ret.pressure = lerp<float>(lower.pressure, upper.pressure, p);
    ret.air_density = lerp<float>(lower.air_density, upper.air_density, p);
    ret.dynamic_viscosity = lerp<float>(lower.dynamic_viscosity,
                                        upper.dynamic_viscosity, p);

    return ret;
}

} // namespace photic
