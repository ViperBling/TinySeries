#pragma once

#include "Geometry.hpp"
#include "Math.hpp"

class Sphere : public Geometry
{
public:
    Sphere(Point3 center, double radius)
        : mCenter(center), mRadius(radius)
    {}

    bool Hit(const Ray& ray, Interval rayT, HitPoint& hitPoint) const override
    {
        Vector3 oc = ray.Origin() - mCenter;
        double a = ray.Direction().LengthSquared();
        double halfB = Dot(oc, ray.Direction());
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
        Vector3 outwardNormal = (hitPoint.mPoint - mCenter) / mRadius;
        hitPoint.SetFaceNormal(ray, outwardNormal);
        return true;
    }

private:
    Point3 mCenter;
    double mRadius;
};