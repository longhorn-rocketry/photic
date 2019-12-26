#ifndef TESTUTIL_H
#define TESTUTIL_H

#include <math.h>
#include <string>
#include <sstream>

#define CHECK_APPROX(k_a, k_b)                                                 \
{                                                                              \
    if (fabs(k_a - k_b) > 1e-4)                                                \
    {                                                                          \
        std::stringstream str;                                                 \
        str << k_a << " is not approximately " << k_b;                         \
        FAIL(str.str().c_str());                                               \
    }                                                                          \
}

#endif
