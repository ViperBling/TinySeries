#pragma once

#include "Utilities/Math.hpp"

namespace Scene
{
    class Ray
    {
    public:
        Ray() = default;
        Ray(const Math::Point3& origin, const Math::Vector3& direction)
            : mOrigin(origin)
            , mDirection(direction)
            , mTime(0.0)
        {}

        Ray(const Math::Point3& origin, const Math::Vector3& direction, double time)
            : mOrigin(origin)
            , mDirection(direction)
            , mTime(time)
        {}

        Math::Point3 Origin() const { return mOrigin; }
        Math::Vector3 Direction() const { return mDirection; }
        double Time() const { return mTime; }

        Math::Point3 At(double t) const
        {
            return mOrigin + mDirection * t;
        }

    private:
        Math::Point3 mOrigin;
        Math::Vector3 mDirection;
        double mTime;
    };
} // namespace Utilities
