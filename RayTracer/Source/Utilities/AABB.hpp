#pragma once

#include "Interval.hpp"
#include "Math.hpp"
#include "Scene/Ray.hpp"

namespace Utilities
{
    class AABB
    {
    public:
        AABB() {}
        AABB(const Interval& x, const Interval& y, const Interval& z)
            : mX(x)
            , mY(y)
            , mZ(z)
        {}
        AABB(const Math::Point3& A, const Math::Point3& B)
        {
            mX = Interval(std::fmin(A[0], B[0]), std::fmax(A[0], B[0]));
            mY = Interval(std::fmin(A[1], B[1]), std::fmax(A[1], B[1]));
            mZ = Interval(std::fmin(A[2], B[2]), std::fmax(A[2], B[2]));
        }

        const Interval& Axis(int n) const
        {
            if (n == 1) return mY;
            if (n == 2) return mZ;
            return mX;
        }

        bool Hit(const Scene::Ray& ray, Interval rayT) const
        {
            for (int i = 0; i < 3; i++)
            {
                auto t0 = std::fmin((Axis(i).mMin - ray.Origin()[i]) / ray.Direction()[i],
                                    (Axis(i).mMax - ray.Origin()[i]) / ray.Direction()[i]);
                auto t1 = std::fmax((Axis(i).mMin - ray.Origin()[i]) / ray.Direction()[i],
                                    (Axis(i).mMax - ray.Origin()[i]) / ray.Direction()[i]);
                rayT.mMin = std::fmax(t0, rayT.mMin);
                rayT.mMax = std::fmin(t1, rayT.mMax);
                if (rayT.mMax <= rayT.mMin)
                {
                    return false;
                }
            }
            return true;
        }

    public:
        Interval mX;
        Interval mY;
        Interval mZ;
    };
}