#include "Camera.hpp"
#include "Scene/Geometry.hpp"
#include "Utilities/Interval.hpp"
#include "Material.hpp"

namespace Scene
{
    void Camera::Initialize(/*int imageWidth, int imageHeight, int samplesPerPixel*/)
    {
        // mImageWidth = imageWidth;
        // mImageHeight = imageHeight;
        mAspectRatio = static_cast<double>(mImageWidth) / mImageHeight;
        // mSamplesPerPixel = samplesPerPixel;

        mOrigin = mLookFrom;

        // 摄像机中心到Viewport的距离
        // float focalLength = (mLookFrom - mLookAt).Length();
        float theta = Utilities::DegreesToRadians(mFov);
        float halfHeight = tan(theta / 2);
        float viewportHeight = 2.0 * halfHeight * mFocusDistance;
        float viewportWidth = mAspectRatio * viewportHeight;

        mW = Math::Normalize(mLookFrom - mLookAt);
        mU = Math::Normalize(Math::Cross(mUp, mW));
        mV = Math::Cross(mW, mU);

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
        Math::Vector3 viewportU = viewportWidth * mU;
        Math::Vector3 viewportV = viewportHeight * -mV;

        // 水平和垂直方向上的像素间隔
        mPixelDeltaU = viewportU / mImageWidth;
        mPixelDeltaV = viewportV / mImageHeight;

        // 计算左上角第一个像素的位置
        auto viewportUpperLeft = mOrigin - (mFocusDistance * mW) - viewportU / 2 - viewportV / 2;
        mUpperLeftCorner = viewportUpperLeft + 0.5 * (mPixelDeltaU + mPixelDeltaV);

        auto defocusRadius = mFocusDistance * std::tan(Utilities::DegreesToRadians(mDefocusAngle) / 2);
        mDefocusDiskU = defocusRadius * mU;
        mDefocusDiskV = defocusRadius * mV;
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
                    pixelColor += RayColor(ray, world, mMaxDepth);
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

        auto rayOrigin = (mDefocusAngle <= 0) ? mOrigin : DefocusDiskSample();
        auto rayDirection = pixelSample - rayOrigin;

        return Ray(rayOrigin, rayDirection);
    }

    Math::Color Camera::RayColor(const Ray &ray, const Geometry &worldHit, int depth) const
    {
        if (depth <= 0)
        {
            return Math::Color(0, 0, 0);
        }
        HitPoint hit;

        if (worldHit.Hit(ray, Utilities::Interval(0.001, Utilities::Infinity), hit))
        {
            Ray scattered;
            Math::Color attenuation;
            if (hit.mMaterial->Scatter(ray, hit, attenuation, scattered))
            {
                return attenuation * RayColor(scattered, worldHit, depth - 1);
            }
            return Math::Color(0, 0, 0);
        }

        Math::Vector3 unitDirection = Math::Normalize(ray.Direction());
        double t = 0.5 * (unitDirection.y() + 1.0);
        return (1.0 - t) * Math::Color(1.0, 1.0, 1.0) + t * Math::Color(0.5, 0.7, 1.0);
    }

    Math::Point3 Camera::DefocusDiskSample() const
    {
        auto p = Math::RandomInUnitDisk();
        return mOrigin + (p[0] * mDefocusDiskU) + (p[1] * mDefocusDiskV);
    }
}
