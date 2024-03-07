#pragma once

#include "Utilities/Utilities.hpp"
#include "Utilities/Math.hpp"

namespace Scene
{
    class Texture
    {
    public:
        Texture() = default;
        ~Texture() = default;
        virtual Math::Color Value(double u, double v, const Math::Point3& point) const = 0;
    };

    class SolidTexture : public Texture
    {
    public:
        SolidTexture() = default;
        SolidTexture(const Math::Color& color)
            : mColor(color)
        {}
        SolidTexture(double r, double g, double b)
            : SolidTexture(Math::Color(r, g, b))
        {}

        Math::Color Value(double u, double v, const Math::Point3& point) const override
        {
            return mColor;
        }

    private:
        Math::Color mColor;
    };

    class CheckerTexture : public Texture
    {
    public:
        CheckerTexture(double scale, std::shared_ptr<Texture> odd, std::shared_ptr<Texture> even)
            : mInvScale(1.0 / scale)
            , mOdd(odd)
            , mEven(even)
        {}
        CheckerTexture(double scale, const Math::Color& odd, const Math::Color& even)
            : mInvScale(1.0 / scale)
            , mOdd(std::make_shared<SolidTexture>(odd))
            , mEven(std::make_shared<SolidTexture>(even))
        {}

        Math::Color Value(double u, double v, const Math::Point3& point) const override
        {
            auto xInteger = static_cast<int>(std::floor(mInvScale * point.x()));
            auto yInteger = static_cast<int>(std::floor(mInvScale * point.y()));
            auto zInteger = static_cast<int>(std::floor(mInvScale * point.z()));

            bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;
            return isEven ? mEven->Value(u, v, point) : mOdd->Value(u, v, point);
        }
    
    private:
        double mInvScale;
        std::shared_ptr<Texture> mOdd;
        std::shared_ptr<Texture> mEven;
    };
}