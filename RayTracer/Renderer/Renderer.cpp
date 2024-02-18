#include "Renderer.hpp"
#include "Math.hpp"
#include "Ray.hpp"

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

            Color pixelColor = ray.RayColor();
            WriteColor(std::cout, pixelColor);
        }
    }
    std::clog << "\rDone.                 \n";
}
