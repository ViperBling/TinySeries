#pragma once

#include "Utilities/Interval.hpp"
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
        bool mbFrontFace;
    };

    class Geometry
    {
    public:
        virtual ~Geometry() = default;
        virtual bool Hit(const Ray& ray, Utilities::Interval rayT, HitPoint& hitPoint) const = 0;
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
        void Add(std::shared_ptr<Geometry> geometry) { mGeometries.push_back(geometry); }

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

    public:
        std::vector<std::shared_ptr<Geometry>> mGeometries;
    };
}
