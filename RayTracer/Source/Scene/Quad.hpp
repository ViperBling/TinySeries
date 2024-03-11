#pragma once

#include "Geometry.hpp"

namespace Scene
{
    /*
     * ^ v
     * |
     * |
     * |
     * |
     * Q --------------> u
    */
    class Quad : public Geometry
    {
    public:
        Quad(const Math::Point3& Q, const Math::Vector3& u, const Math::Vector3& v, std::shared_ptr<Material> material)
            : mQ(Q)
            , mU(u)
            , mV(v)
            , mMaterial(material)
        {
            auto n = Math::Cross(mU, mV);
            mNormal = Math::Normalize(n);
            mD = Dot(mNormal, mQ);
            mW = n / Math::Dot(n, n);
            SetBoundingBox();
        }

        virtual void SetBoundingBox()
        {
            mBoundingBox = AABB(mQ, mQ + mU + mV).Padding();
        }

        AABB BoundingBox() const override
        {
            return mBoundingBox;
        }

        bool Hit(const Ray& ray, Utilities::Interval rayT, HitPoint& hitPoint) const override
        {
            auto denom = Math::Dot(mNormal, ray.Direction());

            if (std::abs(denom) < 1e-6)
            {
                return false;
            }

            auto t = (mD - Math::Dot(mNormal, ray.Origin())) / denom;
            if (!rayT.Contains(t))
            {
                return false;
            }

            auto intersectionPoint = ray.At(t);
            Math::Vector3 planarHitPointVec = intersectionPoint - mQ;
            auto alpha = Math::Dot(mW, Math::Cross(planarHitPointVec, mV));
            auto beta = Math::Dot(mW, Math::Cross(mU, planarHitPointVec));
            if (!IsInterior(alpha, beta, hitPoint))
            {
                return false;
            }

            hitPoint.mT = t;
            hitPoint.mPoint = intersectionPoint;
            hitPoint.mMaterial = mMaterial;
            hitPoint.SetFaceNormal(ray, mNormal);

            return true;
        }

        bool IsInterior(double a, double b, HitPoint& point) const
        {
            if (a < 0 || a > 1 || b < 0 || b > 1)
            {
                return false;
            }
            point.mU = a;
            point.mV = b;
            return true;
        }

    private:
        Math::Point3 mQ;
        Math::Vector3 mU;
        Math::Vector3 mV;
        Math::Vector3 mW;           // 为了测试交点是否在平面范围内，alpha = w · (p x v); beta = w · (u x p)
        Math::Vector3 mNormal;
        double mD;

        std::shared_ptr<Material> mMaterial;
        Scene::AABB mBoundingBox;
    };

    inline std::shared_ptr<GeometryList> Box(const Math::Point3& a, const Math::Point3& b, std::shared_ptr<Scene::Material> mat)
    {
        auto sides = std::make_shared<GeometryList>();

        auto minP = Math::Point3(std::fmin(a.x(), b.x()), std::fmin(a.y(), b.y()), std::fmin(a.z(), b.z()));
        auto maxP = Math::Point3(std::fmax(a.x(), b.x()), std::fmax(a.y(), b.y()), std::fmax(a.z(), b.z()));

        auto dx = Math::Vector3(maxP.x() - minP.x(), 0, 0);
        auto dy = Math::Vector3(0, maxP.y() - minP.y(), 0);
        auto dz = Math::Vector3(0, 0, maxP.z() - minP.z());

        sides->Add(std::make_shared<Scene::Quad>(Math::Point3(minP.x(), minP.y(), maxP.z()),  dx,  dy, mat));
        sides->Add(std::make_shared<Scene::Quad>(Math::Point3(maxP.x(), minP.y(), maxP.z()), -dz,  dy, mat));
        sides->Add(std::make_shared<Scene::Quad>(Math::Point3(maxP.x(), minP.y(), minP.z()), -dx,  dy, mat));
        sides->Add(std::make_shared<Scene::Quad>(Math::Point3(minP.x(), minP.y(), minP.z()),  dz,  dy, mat));
        sides->Add(std::make_shared<Scene::Quad>(Math::Point3(minP.x(), maxP.y(), maxP.z()),  dx, -dz, mat));
        sides->Add(std::make_shared<Scene::Quad>(Math::Point3(minP.x(), minP.y(), minP.z()),  dx,  dz, mat));

        return sides;
    }
}