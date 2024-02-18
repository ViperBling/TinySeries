#include "Renderer.hpp"
#include "Math.hpp"
#include "Ray.hpp"

double HitSphere(const Point3& center, double radius, const Ray& ray)
{
    Vector3 oc = ray.Origin() - center;
    auto a = ray.Direction().LengthSquared();
    auto halfB = Dot(oc, ray.Direction());
    auto c = oc.LengthSquared() - radius * radius;
    auto discriminant = halfB * halfB - a * c;

    if (discriminant < 0)
    {
        return -1.0;
    }
    else
    {
        return (-halfB - sqrt(discriminant)) / a;
    }
}

Color RayColor(const Ray& ray)
{
    auto t = HitSphere(Point3(0, 0, -1), 0.5, ray);
    if (t > 0)
    {
        Vector3 N = Normalize(ray.At(t) - Vector3(0, 0, -1));
        return 0.5 * Color(N.x() + 1, N.y() + 1, N.z() + 1);
    }
   
    Vector3 unitDirection = Normalize(ray.Direction());
    auto a = 0.5 * (unitDirection.y() + 1.0);
    return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
}

void Renderer::Draw()
{
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

            Color pixelColor = RayColor(ray);
            WriteColor(std::cout, pixelColor);
        }
    }
    std::clog << "\rDone.                 \n";
}
