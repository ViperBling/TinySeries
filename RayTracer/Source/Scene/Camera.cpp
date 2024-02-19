#include "Camera.hpp"
#include "Scene/Geometry.hpp"
#include "Utilities/Interval.hpp"

namespace Scene
{
    void Camera::Initialize(int imageWidth, int imageHeight)
    {
        mImageWidth = imageWidth;
        mImageHeight = imageHeight;
        mAspectRatio = static_cast<double>(mImageWidth) / mImageHeight;

        mOrigin = Math::Point3(0, 0, 0);

        float focalLength = 1.0;
        float viewportHeight = 2.0;
        float viewportWidth = mAspectRatio * viewportHeight;

        Math::Vector3 viewportU = Math::Vector3(viewportWidth, 0, 0);
        Math::Vector3 viewportV = Math::Vector3(0, -viewportHeight, 0);

        mPixelDeltaU = viewportU / mImageWidth;
        mPixelDeltaV = viewportV / mImageHeight;

        auto viewportUpperLeft = mOrigin - Math::Vector3(0, 0, focalLength) - viewportU / 2 - viewportV / 2;
        mUpperLeftCorner = viewportUpperLeft + 0.5 * (mPixelDeltaU + mPixelDeltaV);
    }

    void Camera::Render(const Scene::Geometry& world)
    {
        std::cout << "P3\n" << mImageWidth << " " << mImageHeight << "\n255\n";

        for (int j = 0; j < mImageHeight; j++)
        {
            std::clog << "\rScanlines remaining: " << mImageHeight - j << " " << std::flush;
            for (int i = 0; i < mImageWidth; i++)
            {
                auto pixelCenter = mUpperLeftCorner + i * mPixelDeltaU + j * mPixelDeltaV;
                auto rayDir = pixelCenter - mOrigin;
                Ray ray(mOrigin, rayDir);

                Math::Color pixelColor = RayColor(ray, world);
                Math::WriteColor(std::cout, pixelColor);
            }
        }
        std::clog << "\rDone.                 \n";
    }

    Math::Color Camera::RayColor(const Ray &ray, const Geometry &worldHit) const
    {
        HitPoint hit;

        if (worldHit.Hit(ray, Utilities::Interval(0, Utilities::Infinity), hit))
        {
            return 0.5 * (hit.mNormal + Math::Color(1, 1, 1));
        }

        Math::Vector3 unitDirection = Math::Normalize(ray.Direction());
        double t = 0.5 * (unitDirection.y() + 1.0);
        return (1.0 - t) * Math::Color(1.0, 1.0, 1.0) + t * Math::Color(0.5, 0.7, 1.0);
    }
}

