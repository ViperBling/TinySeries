#pragma once

#include "Math.hpp"

namespace Utilities
{
    class PerlinNoise
    {
    public:
        PerlinNoise();
        ~PerlinNoise();

        double Noise(const Math::Point3& p) const;
        double Turb(const Math::Point3& p, int depth = 7) const;

    private:
        static int* PerlinGeneratePerm();
        static void Permute(int* p, int n);
        static double PerlinInterp(Math::Vector3 c[2][2][2], double u, double v, double w);

    private:
        static const int mPointCount = 256;
        Math::Vector3* mRandomVector;
        int* mPermX;
        int* mPermY;
        int* mPermZ;
    };
}