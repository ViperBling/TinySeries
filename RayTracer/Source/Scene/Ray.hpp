#pragma once

#include "Utilities/Math.hpp"

namespace Scene
{
    class Ray
    {
    public:
        Ray() = default;
        Ray(const Math::Point3& origin, const Math::Vector3& direction)
            : mOrigin(origin), mDirection(direction)
        {}

        Math::Point3 Origin() const { return mOrigin; }
        Math::Vector3 Direction() const { return mDirection; }

        Math::Point3 At(double t) const
        {
            return mOrigin + mDirection * t;
        }

    private:
        Math::Point3 mOrigin;
        Math::Vector3 mDirection;
    };
} // namespace Utilities
