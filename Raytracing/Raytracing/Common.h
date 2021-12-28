#ifndef COMMON_H
#define COMMON_H

#include <limits>
#include <cstdlib>

#include "Ray.h"
#include "Vec3.h"

////////////////////////////////
// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

////////////////////////////////
// Utility Functions
inline double degrees_to_radians(double degrees)
{
    return degrees * pi / 180.0;
}

// [0, 1)
inline double random_double()
{
    return rand() / (RAND_MAX + 1.0);
}

// [min, max)
inline double random_double(double min, double max)
{
    return min + (max - min) * random_double();
}

#endif