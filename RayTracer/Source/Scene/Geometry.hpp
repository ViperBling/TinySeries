#pragma once

#include "Utilities/Interval.hpp"
#include "Scene/AABB.hpp"
#include "Scene/Ray.hpp"

#include <memory>
#include <vector>

namespace Scene
{
    class Material;
    
    class HitPoint
    {
    public:
        void SetFaceNormal(const Ray& ray, const Math::Vector3& outwardNormal)
        {
            mbFrontFace = Dot(ray.Direction(), outwardNormal) < 0;
            mNormal = mbFrontFace ? outwardNormal : -outwardNormal;
        }

    public:
        Math::Point3 mPoint;
        Math::Vector3 mNormal;
        std::shared_ptr<Material> mMaterial;
        double mT;
        double mU;
        double mV;
        bool mbFrontFace;
    };

    class Geometry
    {
    public:
        virtual ~Geometry() = default;
        virtual bool Hit(const Ray& ray, Utilities::Interval rayT, HitPoint& hitPoint) const = 0;
        virtual Scene::AABB BoundingBox() const = 0;
    };

    class GeometryList : public Geometry
    {
    public:
        GeometryList() = default;
        GeometryList(std::shared_ptr<Geometry> geometry)
        {
            Add(geometry);
        }

        void Clear() { mGeometries.clear(); }
        void Add(std::shared_ptr<Geometry> geometry) 
        { 
            mGeometries.push_back(geometry); 
            mBoundingBox = Scene::AABB(mBoundingBox, geometry->BoundingBox());
        }

        bool Hit(const Ray& ray, Utilities::Interval rayT, HitPoint& hitPoint) const override
        {
            HitPoint tempHitPoint;
            bool hitAnything = false;
            double closestSoFar = rayT.mMax;

            for (const auto& geometry : mGeometries)
            {
                if (geometry->Hit(ray, Utilities::Interval(rayT.mMin, closestSoFar), tempHitPoint))
                {
                    hitAnything = true;
                    closestSoFar = tempHitPoint.mT;
                    hitPoint = tempHitPoint;
                }
            }
            return hitAnything;
        }

        Scene::AABB BoundingBox() const override
        {
            return mBoundingBox;
        }

    public:
        std::vector<std::shared_ptr<Geometry>> mGeometries;
    private:
        Scene::AABB mBoundingBox;
    };

    class Translate : public Geometry
    {
    public:
        Translate(std::shared_ptr<Geometry> geometry, const Math::Vector3& offset)
            : mGeometry(geometry)
            , mOffset(offset)
        {
            mBoundingBox = mGeometry->BoundingBox() + mOffset;
        }

        bool Hit(const Ray& ray, Utilities::Interval rayT, HitPoint& hitPoint) const override
        {
            // Move the ray backwards by the offset
            Ray offsetTrans(ray.Origin() - mOffset, ray.Direction(), ray.Time());

            // Determine where (if any) an intersection occurs along the offset ray
            if (!mGeometry->Hit(offsetTrans, rayT, hitPoint))
            {
                return false;
            }
            // Move the intersection point forwards by the offset
            hitPoint.mPoint += mOffset;
            return true;
        }

        Scene::AABB BoundingBox() const override
        {
            return mBoundingBox;
        }

    private:
        std::shared_ptr<Geometry> mGeometry;
        Math::Vector3 mOffset;
        Scene::AABB mBoundingBox;
    };

    class RotateY : public Geometry
    {
    public:
        RotateY(std::shared_ptr<Geometry> object, double angle) : mGeometry(object)
        {
            auto radians = Math::DegreesToRadians(angle);
            mSinTheta = std::sin(radians);
            mCosTheta = std::cos(radians);
            mBoundingBox = mGeometry->BoundingBox();

            Math::Point3 minPoint( Math::Infinity,  Math::Infinity,  Math::Infinity);
            Math::Point3 maxPoint(-Math::Infinity, -Math::Infinity, -Math::Infinity);

            for (int i = 0; i < 2; i++)
            {
                for (int j = 0; j < 2; j++)
                {
                    for (int k = 0; k < 2; k++)
                    {
                        auto x = i * mBoundingBox.mX.mMax + (1 - i) * mBoundingBox.mX.mMin;
                        auto y = j * mBoundingBox.mY.mMax + (1 - j) * mBoundingBox.mY.mMin;
                        auto z = k * mBoundingBox.mZ.mMax + (1 - k) * mBoundingBox.mZ.mMin;

                        auto newX =  mCosTheta * x + mSinTheta * z;
                        auto newZ = -mSinTheta * x + mCosTheta * z;

                        Math::Vector3 tester(newX, y, newZ);

                        for (int c = 0; c < 3; c++)
                        {
                            minPoint[c] = std::fmin(minPoint[c], tester[c]);
                            maxPoint[c] = std::fmax(maxPoint[c], tester[c]);
                        }
                    }
                }
            }
            mBoundingBox = Scene::AABB(minPoint, maxPoint);
        }

        bool Hit(const Ray& ray, Utilities::Interval rayT, HitPoint& hitPoint) const override
        {
            // Change the ray from world space to object space
            auto origin = ray.Origin();
            auto direction = ray.Direction();

            origin[0] = mCosTheta * ray.Origin()[0] - mSinTheta * ray.Origin()[2];
            origin[2] = mSinTheta * ray.Origin()[0] + mCosTheta * ray.Origin()[2];
            direction[0] = mCosTheta * ray.Direction()[0] - mSinTheta * ray.Direction()[2];
            direction[2] = mSinTheta * ray.Direction()[0] + mCosTheta * ray.Direction()[2];

            Ray rotatedRay(origin, direction, ray.Time());

            // Determine whether an intersection exists in object space (and if so, where)
            if (!mGeometry->Hit(rotatedRay, rayT, hitPoint))
            {
                return false;
            }

            // Change the intersection point from object space to world space
            auto p = hitPoint.mPoint;
            p[0] =  mCosTheta * hitPoint.mPoint[0] + mSinTheta * hitPoint.mPoint[2];
            p[2] = -mSinTheta * hitPoint.mPoint[0] + mCosTheta * hitPoint.mPoint[2];
            
            // Change the normal from object space to world space
            auto normal = hitPoint.mNormal;
            normal[0] =  mCosTheta * hitPoint.mNormal[0] + mSinTheta * hitPoint.mNormal[2];
            normal[2] = -mSinTheta * hitPoint.mNormal[0] + mCosTheta * hitPoint.mNormal[2];

            hitPoint.mPoint = p;
            hitPoint.mNormal = normal;

            return true;
        }

        Scene::AABB BoundingBox() const override
        {
            return mBoundingBox;
        }

    private:
        std::shared_ptr<Geometry> mGeometry;
        double mSinTheta;
        double mCosTheta;
        Scene::AABB mBoundingBox;
    };
}
