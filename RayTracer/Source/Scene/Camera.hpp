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
        void Initialize(/*int imageWidth, int imageHeight, int samplesPerPixel*/);
        void Render(const Scene::Geometry& world);

    private:
        Math::Vector3 PixelSampleSquare() const;
        Ray GetRay(int i, int j) const;
        Math::Color RayColor(const Ray& ray, const Geometry& worldHit, int depth) const;
        Math::Point3 DefocusDiskSample() const;

    public:
        double mAspectRatio = 1.0;
        int mImageWidth = 100;
        int mImageHeight = 100;
        int mSamplesPerPixel = 10;
        int mMaxDepth = 10;

        double mFov = 90;
        Math::Point3 mLookFrom = Math::Point3(0, 0, -1);
        Math::Point3 mLookAt = Math::Point3(0, 0, 0);
        Math::Vector3 mUp = Math::Vector3(0, 1, 0);

        double mDefocusAngle = 0;
        double mFocusDistance = 10;

    private:
        Math::Point3 mOrigin;
        Math::Point3 mUpperLeftCorner;
        Math::Vector3 mPixelDeltaU;
        Math::Vector3 mPixelDeltaV;
        Math::Vector3 mU, mV, mW;

        Math::Vector3 mDefocusDiskU;
        Math::Vector3 mDefocusDiskV;
    };
} // namespace Scene

