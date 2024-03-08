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
        scattered = Ray(hitPoint.mPoint, scatterDirection, ray.Time());
        attenuation = mAlbedo->Value(hitPoint.mU, hitPoint.mV, hitPoint.mPoint);
        return true;
    }

    bool Metal::Scatter(const Ray &ray, const HitPoint &hitPoint, Math::Color &attenuation, Ray &scattered) const
    {
        Math::Vector3 reflected = Math::Reflect(Math::Normalize(ray.Direction()), hitPoint.mNormal);
        scattered = Ray(hitPoint.mPoint, reflected + mFuzz * Math::RandomUnitVector(), ray.Time());
        attenuation = mAlbedo;
        return (Math::Dot(scattered.Direction(), hitPoint.mNormal) > 0);
    }

    bool Dielectric::Scatter(const Ray &ray, const HitPoint &hitPoint, Math::Color &attenuation, Ray &scattered) const
    {
        attenuation = Math::Color(1.0, 1.0, 1.0);
        double refractionRatio = hitPoint.mbFrontFace ? (1.0 / mRefraction) : mRefraction;

        Math::Vector3 unitDirection = Math::Normalize(ray.Direction());
        double cosTheta = std::fmin(Math::Dot(-unitDirection, hitPoint.mNormal), 1.0);
        double sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);

        bool cannotRefract = refractionRatio * sinTheta > 1.0;
        Math::Vector3 direction;
        if (cannotRefract || Reflectance(cosTheta, refractionRatio) > Math::RandomDouble())
        {
            direction = Math::Reflect(unitDirection, hitPoint.mNormal);
        }
        else
        {
            direction = Math::Refract(unitDirection, hitPoint.mNormal, refractionRatio);
        }
        scattered = Ray(hitPoint.mPoint, direction, ray.Time());
        return true;
    }
}