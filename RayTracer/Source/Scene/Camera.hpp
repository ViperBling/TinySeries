#pragma once

#include "Utilities/Math.hpp"

namespace Scene
{
    class Ray;
    class Geometry;
}

namespace Scene
{
    class Camera
    {
    public:
        Camera() = default;
        void Initialize(int imageWidth, int imageHeight);
        void Render(const Scene::Geometry& world);

    private:
        Math::Color RayColor(const Ray& ray, const Geometry& worldHit) const;

    public:
        double mAspectRatio = 1.0;
        int mImageWidth = 100;
        int mImageHeight = 100;

    private:
        Math::Point3 mOrigin;
        Math::Point3 mUpperLeftCorner;
        Math::Vector3 mPixelDeltaU;
        Math::Vector3 mPixelDeltaV;
    };
} // namespace Scene

