#pragma once

#include <cmath>
#include <limits>
#include <memory>

const double Infinity = std::numeric_limits<double>::infinity();
const double Pi = 3.1415926535897932385;

inline double DegreesToRadians(double degrees)
{
    return degrees * Pi / 180.0;
}

#include "Ray.hpp"
#include "Math.hpp"