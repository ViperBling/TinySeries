#pragma once

#include <cmath>
#include <cstdlib>
#include <random>
#include <limits>
#include <memory>

namespace Utilities
{
    const double Infinity = std::numeric_limits<double>::infinity();
    const double Pi = 3.1415926535897932385;

    inline double DegreesToRadians(double degrees)
    {
        return degrees * Pi / 180.0;
    }

    inline double RandomDouble()
    {
        static std::uniform_real_distribution<double> distribution(0.0, 1.0);
        static std::mt19937 generator;
        return distribution(generator);
    }

    inline double RandomDouble(double min, double max)
    {
        return min + (max - min) * RandomDouble();
    }
}