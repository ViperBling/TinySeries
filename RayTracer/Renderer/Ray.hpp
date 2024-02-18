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

    Color RayColor()
    {
        Vector3 unitDirection = Normalize(Direction());
        auto t = 0.5 * (unitDirection.y() + 1.0);
        return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
    }

private:
    Point3 mOrigin;
    Vector3 mDirection;
};