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

            hitPoint.mT = t;
            hitPoint.mPoint = intersectionPoint;
            hitPoint.mMaterial = mMaterial;
            hitPoint.SetFaceNormal(ray, mNormal);

            return true;
        }

    private:
        Math::Point3 mQ;
        Math::Vector3 mU;
        Math::Vector3 mV;
        Math::Vector3 mW;
        Math::Vector3 mNormal;
        double mD;

        std::shared_ptr<Material> mMaterial;
        Scene::AABB mBoundingBox;
    };
}