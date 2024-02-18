#pragma once

#include "Math.hpp"

class Ray
{
public:
    Ray() = default;
    Ray(const Point3& origin, const Vector3& direction)
        : mOrigin(origin), mDirection(direction)
    {}

    Point3 Origin() const { return mOrigin; }
    Vector3 Direction() const { return mDirection; }

    Point3 At(double t) const
    {
        return mOrigin + mDirection * t;
    }

private:
    Point3 mOrigin;
    Vector3 mDirection;
};