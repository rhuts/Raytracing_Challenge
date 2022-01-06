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


#endif