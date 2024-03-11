#pragma once

#include "Material.hpp"

namespace Scene
{
    class DiffuseLight : public Material
    {
    public:
        DiffuseLight(std::shared_ptr<Texture> emit)
            : mEmit(emit)
        {}
        DiffuseLight(const Math::Color& color)
            : mEmit(std::make_shared<SolidTexture>(color))
        {}

        bool Scatter(const Ray& ray, const HitPoint& hitPoint, Math::Color& attenuation, Ray& scattered) const override
        {
            return false;
        }

        Math::Color Emitted(double u, double v, const Math::Point3& point) const override
        {
            return mEmit->Value(u, v, point);
        }

    private:
        std::shared_ptr<Texture> mEmit;
    };
}