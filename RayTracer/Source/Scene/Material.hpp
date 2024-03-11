#pragma once

#include "Utilities/Math.hpp"
#include "Texture.hpp"

namespace Scene
{
    class HitPoint;
    class Ray;

    class Material
    {
    public:
        virtual ~Material() = default;
        virtual bool Scatter(const Ray& ray, const HitPoint& hitPoint, Math::Color& attenuation, Ray& scattered) const = 0;
        virtual Math::Color Emitted(double u, double v, const Math::Point3& point) const
        {
            return Math::Color(0, 0, 0);
        }
    };

    class Lambertian : public Material
    {
    public:
        Lambertian(const Math::Color& albedo)
            : mAlbedo(std::make_shared<SolidTexture>(albedo))
        {}
        Lambertian(std::shared_ptr<Texture> albedo)
            : mAlbedo(albedo)
        {}
        
        bool Scatter(const Ray& ray, const HitPoint& hitPoint, Math::Color& attenuation, Ray& scattered) const override;

    private:
        std::shared_ptr<Texture> mAlbedo;
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

    class Isotropic : public Material
    {
    public:
        Isotropic(const Math::Color& color)
            : mAlbedo(std::make_shared<SolidTexture>(color))
        {}
        Isotropic(std::shared_ptr<Texture> albedo)
            : mAlbedo(albedo)
        {}
        bool Scatter(const Ray& ray, const HitPoint& hitPoint, Math::Color& attenuation, Ray& scattered) const override;
    
    private:
        std::shared_ptr<Texture> mAlbedo;
    };
}