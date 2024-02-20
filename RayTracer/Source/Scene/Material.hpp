#pragma once

#include "Utilities/Utilities.hpp"
#include "Utilities/Math.hpp"

namespace Scene
{
    class HitPoint;
    class Ray;

    class Material
    {
    public:
        virtual ~Material() = default;
        virtual bool Scatter(const Ray& ray, const HitPoint& hitPoint, Math::Color& attenuation, Ray& scattered) const = 0;
    };

    class Lambertian : public Material
    {
    public:
        Lambertian(const Math::Color& albedo)
            : mAlbedo(albedo)
        {}
        bool Scatter(const Ray& ray, const HitPoint& hitPoint, Math::Color& attenuation, Ray& scattered) const override;

    private:
        Math::Color mAlbedo;
    };

    class Metal : public Material
    {
    public:
        Metal(const Math::Color& albedo, double fuzz)
            : mAlbedo(albedo)
            , mFuzz(fuzz < 1 ? fuzz : 1)
        {}
        bool Scatter(const Ray& ray, const HitPoint& hitPoint, Math::Color& attenuation, Ray& scattered) const override;

    private:
        Math::Color mAlbedo;
        double mFuzz;
    };

    class Dielectric : public Material
    {
    public:
        Dielectric(double refraction)
            : mRefraction(refraction)
        {}
        bool Scatter(const Ray& ray, const HitPoint& hitPoint, Math::Color& attenuation, Ray& scattered) const override;
        static double Reflectance(double cosine, double refIdx)
        {
            // Use Schlick's approximation for reflectance.
            double r0 = (1 - refIdx) / (1 + refIdx);
            r0 = r0 * r0;
            return r0 + (1 - r0) * std::pow((1 - cosine), 5);
        }

    private:
        double mRefraction;
    };
}