#include "Material.hpp"
#include "Geometry.hpp"

namespace Scene
{
    bool Lambertian::Scatter(const Ray& ray, const HitPoint& hitPoint, Math::Color& attenuation, Ray& scattered) const
    {
        auto scatterDirection = hitPoint.mNormal + Math::RandomUnitVector();
        if (scatterDirection.NearZero())
        {
            scatterDirection = hitPoint.mNormal;
        }
        scattered = Ray(hitPoint.mPoint, scatterDirection);
        attenuation = mAlbedo;
        return true;
    }

    bool Metal::Scatter(const Ray &ray, const HitPoint &hitPoint, Math::Color &attenuation, Ray &scattered) const
    {
        Math::Vector3 reflected = Math::Reflect(Math::Normalize(ray.Direction()), hitPoint.mNormal);
        scattered = Ray(hitPoint.mPoint, reflected + mFuzz * Math::RandomUnitVector());
        attenuation = mAlbedo;
        return (Math::Dot(scattered.Direction(), hitPoint.mNormal) > 0);
    }
}