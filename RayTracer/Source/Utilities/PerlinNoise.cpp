#include "PerlinNoise.hpp"

namespace Utilities
{
    PerlinNoise::PerlinNoise()
    {
        mRandomVector = new Math::Vector3[mPointCount];
        for (int i = 0; i < mPointCount; ++i)
        {
            mRandomVector[i] = Math::Normalize(Math::Vector3::Random(-1, 1));
        }
        mPermX = PerlinGeneratePerm();
        mPermY = PerlinGeneratePerm();
        mPermZ = PerlinGeneratePerm();
    }

    PerlinNoise::~PerlinNoise()
    {
        delete[] mRandomVector;
        delete[] mPermX;
        delete[] mPermY;
        delete[] mPermZ;
    }

    double PerlinNoise::Noise(const Math::Point3 &p) const
    {
        auto u = p.x() - std::floor(p.x());
        auto v = p.y() - std::floor(p.y());
        auto w = p.z() - std::floor(p.z());
        auto i = static_cast<int>(std::floor(p.x()));
        auto j = static_cast<int>(std::floor(p.y()));
        auto k = static_cast<int>(std::floor(p.z()));
        Math::Vector3 c[2][2][2];

        for (int di = 0; di < 2; ++di)
        {
            for (int dj = 0; dj < 2; ++dj)
            {
                for (int dk = 0; dk < 2; ++dk)
                {
                    c[di][dj][dk] = mRandomVector[
                        mPermX[(i + di) & 255] ^
                        mPermY[(j + dj) & 255] ^
                        mPermZ[(k + dk) & 255]
                    ];
                }
            }
        }
        return PerlinInterp(c, u, v, w);
    }

    double PerlinNoise::Turb(const Math::Point3 &p, int depth) const
    {
        auto accumulate = 0.0;
        auto tempP = p;
        auto weight = 1.0;

        for (int i = 0; i < depth; ++i)
        {
            accumulate += weight * Noise(tempP);
            weight *= 0.5;
            tempP *= 2;
        }
        return std::fabs(accumulate);
    }

    int *PerlinNoise::PerlinGeneratePerm()
    {
        auto p = new int[mPointCount];
        for (int i = 0; i < mPointCount; ++i)
        {
            p[i] = i;
        }
        Permute(p, mPointCount);
        return p;
    }

    void PerlinNoise::Permute(int *p, int n)
    {
        for (int i = n - 1; i > 0; --i)
        {
            int target = Math::RandomInt(0, i);
            int temp = p[i];
            p[i] = p[target];
            p[target] = temp;
        }
    }

    double PerlinNoise::PerlinInterp(Math::Vector3 c[2][2][2], double u, double v, double w)
    {
        auto uu = u * u * (3 - 2 * u);
        auto vv = v * v * (3 - 2 * v);
        auto ww = w * w * (3 - 2 * w);
        auto accumulate = 0.0;

        for (int i = 0; i < 2; ++i)
        {
            for (int j = 0; j < 2; ++j)
            {
                for (int k = 0; k < 2; ++k)
                {
                    Math::Point3 weightV(u - i, v - j, w - k);
                    accumulate += (i * uu + (1 - i) * (1 - uu)) *
                                  (j * vv + (1 - j) * (1 - vv)) *
                                  (k * ww + (1 - k) * (1 - ww)) *
                                  Math::Dot(c[i][j][k], weightV);
                }
            }
        }
        return accumulate;
    }
}