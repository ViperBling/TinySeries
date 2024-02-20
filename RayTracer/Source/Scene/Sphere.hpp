#pragma once

#include "Geometry.hpp"
#include "Utilities/Math.hpp"

namespace Scene
{
    class Sphere : public Geometry
    {
    public:
        Sphere(Math::Point3 center, double radius, std::shared_ptr<Material> material)
            : mCenter(center)
            , mRadius(radius)
            , mMaterial(material)
        {}

        bool Hit(const Ray& ray, Utilities::Interval rayT, HitPoint& hitPoint) const override
        {
            Math::Vector3 oc = ray.Origin() - mCenter;
            double a = ray.Direction().LengthSquared();
            double halfB = Math::Dot(oc, ray.Direction());
            double c = oc.LengthSquared() - mRadius * mRadius;
            double discriminant = halfB * halfB - a * c;
            if (discriminant < 0)
            {
                return false;
            }
            double sqrtd = std::sqrt(discriminant);
            // 求一元二次方程的两个根，判断是否在射线范围内
            double root = (-halfB - sqrtd) / a;
            if (!rayT.Surrounds(root))
            {
                root = (-halfB + sqrtd) / a;
                if (!rayT.Surrounds(root))
                {
                    return false;
                }
            }
            hitPoint.mT = root;
            hitPoint.mPoint = ray.At(hitPoint.mT);
            Math::Vector3 outwardNormal = (hitPoint.mPoint - mCenter) / mRadius;
            hitPoint.SetFaceNormal(ray, outwardNormal);
            hitPoint.mMaterial = mMaterial;
            return true;
        }

    private:
        Math::Point3 mCenter;
        double mRadius;
        std::shared_ptr<Material> mMaterial;
    };
}
