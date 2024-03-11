#pragma once

#include "Utilities/Interval.hpp"
#include "Utilities/Math.hpp"
#include "Ray.hpp"

namespace Scene
{
    class AABB
    {
    public:
        AABB() {}
        AABB(const Utilities::Interval& x, const Utilities::Interval& y, const Utilities::Interval& z)
            : mX(x)
            , mY(y)
            , mZ(z)
        {}
        AABB(const Math::Point3& A, const Math::Point3& B)
        {
            mX = Utilities::Interval(std::fmin(A[0], B[0]), std::fmax(A[0], B[0]));
            mY = Utilities::Interval(std::fmin(A[1], B[1]), std::fmax(A[1], B[1]));
            mZ = Utilities::Interval(std::fmin(A[2], B[2]), std::fmax(A[2], B[2]));
        }
        AABB(const AABB& box0, const AABB& box1)
        {
            mX = Utilities::Interval(box0.mX, box1.mX);
            mY = Utilities::Interval(box0.mY, box1.mY);
            mZ = Utilities::Interval(box0.mZ, box1.mZ);
        }

        AABB Padding()
        {
            double delta = 0.0001;
            Utilities::Interval newX = (mX.Size() >= delta) ? mX : mX.Expand(delta);
            Utilities::Interval newY = (mY.Size() >= delta) ? mY : mY.Expand(delta);
            Utilities::Interval newZ = (mZ.Size() >= delta) ? mZ : mZ.Expand(delta);
            return AABB(newX, newY, newZ);
        }

        const Utilities::Interval& Axis(int n) const
        {
            if (n == 1) return mY;
            if (n == 2) return mZ;
            return mX;
        }

        bool Hit(const Scene::Ray& ray, Utilities::Interval rayT) const
        {
            for (int i = 0; i < 3; i++)
            {
                auto invDir = 1 / ray.Direction()[i];
                auto origin = ray.Origin()[i];

                auto t0 = (Axis(i).mMin - origin) * invDir;
                auto t1 = (Axis(i).mMax - origin) * invDir;

                if (invDir < 0)
                {
                    std::swap(t0, t1);
                }
                if (t0 > rayT.mMin) { rayT.mMin = t0; }
                if (t1 < rayT.mMax) { rayT.mMax = t1; }
                if (rayT.mMax <= rayT.mMin)
                {
                    return false;
                }
            }
            return true;
        }

    public:
        // 三个维度的区间
        Utilities::Interval mX;
        Utilities::Interval mY;
        Utilities::Interval mZ;
    };
    
    inline AABB operator+(const AABB& box, const Math::Point3& offset)
    {
        return AABB(box.mX + offset.x(), box.mY + offset.y(), box.mZ + offset.z());
    }

    inline AABB operator+(const Math::Point3& offset, const AABB& box)
    {
        return box + offset;
    }
}