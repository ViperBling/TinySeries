#pragma once

#include "Geometry.hpp"
#include "Material.hpp"
#include "Utilities/Math.hpp"

namespace Scene
{
    class ConstantMedium : public Geometry
    {
    public:
        ConstantMedium(std::shared_ptr<Geometry> boundary, double density, std::shared_ptr<Texture> phaseFunction)
            : mBoundary(boundary)
            , mNegInvDensity(-1 / density)
            , mPhaseFunction(std::make_shared<Isotropic>(phaseFunction))
        {}
        ConstantMedium(std::shared_ptr<Geometry> boundary, double density, Math::Color color)
            : mBoundary(boundary)
            , mNegInvDensity(-1 / density)
            , mPhaseFunction(std::make_shared<Isotropic>(color))
        {}

        bool Hit(const Ray& ray, Utilities::Interval rayT, HitPoint& hitPoint) const override
        {
            const bool enableDebug = false;
            const bool debugging = enableDebug && Math::RandomDouble() < 0.00001;

            HitPoint hit1, hit2;

            if (!mBoundary->Hit(ray, Utilities::Interval::Universe, hit1))
            {
                return false;
            }
            if (!mBoundary->Hit(ray, Utilities::Interval(hit1.mT + 0.0001, Math::Infinity), hit2))
            {
                return false;
            }

            if (debugging) std::clog << "\ntMin = " << hit1.mT << ", tMax =  " << hit2.mT << "\n";

            if (hit1.mT < rayT.mMin) hit1.mT = rayT.mMin;
            if (hit2.mT > rayT.mMax) hit2.mT = rayT.mMax;

            if (hit1.mT >= hit2.mT)
            {
                return false;
            }
            if (hit1.mT < 0)
            {
                hit1.mT = 0;
            }

            auto rayLength = ray.Direction().Length();
            auto distanceInsideBoundary = (hit2.mT - hit1.mT) * rayLength;
            auto hitDistance = mNegInvDensity * std::log(Math::RandomDouble());

            if (hitDistance > distanceInsideBoundary)
            {
                return false;
            }

            hitPoint.mT = hit1.mT + hitDistance / rayLength;
            hitPoint.mPoint = ray.At(hitPoint.mT);

            if (debugging)
            {
                std::clog << "HitDistance = " << hitDistance << "\n"
                          << "hit.mT = " << hitPoint.mT << "\n"
                          << "hit.mPoint = " << hitPoint.mPoint << "\n";
            }
            hitPoint.mNormal = Math::Vector3(1, 0, 0);
            hitPoint.mbFrontFace = true;
            hitPoint.mMaterial = mPhaseFunction;

            return true;
        }

        Scene::AABB BoundingBox() const override
        {
            return mBoundary->BoundingBox();
        }
    
    private:
        std::shared_ptr<Geometry> mBoundary;
        double mNegInvDensity;
        std::shared_ptr<Material> mPhaseFunction;
    };
}