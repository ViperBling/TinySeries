#include "Camera.hpp"
#include "Scene/Geometry.hpp"
#include "Utilities/Interval.hpp"

namespace Scene
{
    void Camera::Initialize(int imageWidth, int imageHeight, int samplesPerPixel)
    {
        mImageWidth = imageWidth;
        mImageHeight = imageHeight;
        mAspectRatio = static_cast<double>(mImageWidth) / mImageHeight;
        mSamplesPerPixel = samplesPerPixel;

        mOrigin = Math::Point3(0, 0, 0);

        // 摄像机中心到Viewport的距离
        float focalLength = 1.0;
        float viewportHeight = 2.0;
        float viewportWidth = mAspectRatio * viewportHeight;

        /*
         * -----------------> u
         * | P00 
         * | 
         * | 
         * | 
         * V v
         * 整体如上，左上角为第一个像素P00，依次向右和下递增
        */
        // Viewport的水平和垂直方向
        Math::Vector3 viewportU = Math::Vector3(viewportWidth, 0, 0);
        Math::Vector3 viewportV = Math::Vector3(0, -viewportHeight, 0);

        // 水平和垂直方向上的像素间隔
        mPixelDeltaU = viewportU / mImageWidth;
        mPixelDeltaV = viewportV / mImageHeight;

        // 计算左上角第一个像素的位置
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
                Math::Color pixelColor(0, 0, 0);
                for (int s = 0; s < mSamplesPerPixel; s++)
                {
                    Ray ray = GetRay(i, j);
                    pixelColor += RayColor(ray, world);
                }
                Math::WriteColor(std::cout, pixelColor, mSamplesPerPixel);
            }
        }
        std::clog << "\rDone.                 \n";
    }

    Math::Vector3 Camera::PixelSampleSquare() const
    {
        auto px = -0.5 + Utilities::RandomDouble();
        auto py = -0.5 + Utilities::RandomDouble();
        return (px * mPixelDeltaU + py * mPixelDeltaV);
    }

    Ray Camera::GetRay(int i, int j) const
    {
        auto pixelCenter = mUpperLeftCorner + i * mPixelDeltaU + j * mPixelDeltaV;
        auto pixelSample = pixelCenter + PixelSampleSquare();

        auto rayOrigin = mOrigin;
        auto rayDirection = pixelSample - mOrigin;

        return Ray(rayOrigin, rayDirection);
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

