#include "Renderer.hpp"
#include "Utilities.hpp"
#include "Geometry.hpp"
#include "Sphere.hpp"

Color RayColor(const Ray& ray, const Geometry& worldHit)
{
    HitPoint hitPoint;
    if (worldHit.Hit(ray, Interval(0, Infinity), hitPoint))
    {
        return 0.5 * (hitPoint.mNormal + Color(1, 1, 1));
    }
   
    Vector3 unitDirection = Normalize(ray.Direction());
    auto a = 0.5 * (unitDirection.y() + 1.0);
    return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
}

void Renderer::Draw()
{
    GeometryList world;
    world.Add(std::make_shared<Sphere>(Point3(0, 0, -1), 0.5));
    world.Add(std::make_shared<Sphere>(Point3(0, -100.5, -1), 100));

    // Camera
    float focalLength = 1.0;
    float viewportHeight = 2.0;
    float viewportWidth = mAspectRatio * viewportHeight;

    Point3 cameraPos = Point3(0, 0, 0);

    Vector3 viewportU = Vector3(viewportWidth, 0, 0);
    Vector3 viewportV = Vector3(0, -viewportHeight, 0);

    auto pixelDeltaU = viewportU / mImageWidth;
    auto pixelDeltaV = viewportV / mImageHeight;

    auto viewportUpperLeft = cameraPos - Vector3(0, 0, focalLength) - viewportU / 2 - viewportV / 2;
    auto pixel00Location = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);

    std::cout << "P3\n" << mImageWidth << " " << mImageHeight << "\n255\n";

    for (int j = 0; j < mImageHeight; j++)
    {
        std::clog << "\rScanlines remaining: " << mImageHeight - j << " " << std::flush;
        for (int i = 0; i < mImageWidth; i++)
        {
            auto pixelCenter = pixel00Location + i * pixelDeltaU + j * pixelDeltaV;
            auto rayDir = pixelCenter - cameraPos;
            Ray ray(cameraPos, rayDir);

            Color pixelColor = RayColor(ray, world);
            WriteColor(std::cout, pixelColor);
        }
    }
    std::clog << "\rDone.                 \n";
}
