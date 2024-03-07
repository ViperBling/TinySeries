#pragma once

#include "Utilities/Utilities.hpp"
#include "Scene/Geometry.hpp"

namespace Scene
{
    class BVHNode : public Geometry
    {
    public:
        BVHNode(const std::vector<std::shared_ptr<Geometry>>& srcObjs, size_t start, size_t end)
        {
            auto objects = srcObjs;

            int axis = Math::RandomInt(0, 2);
            auto comparator = (axis == 0) ? BoxCompareX : (axis == 1) ? BoxCompareY : BoxCompareZ;

            size_t objectSpan = end - start;
            if (objectSpan == 1)
            {
                mLeft = mRight = objects[start];
            }
            else if (objectSpan == 2)
            {
                if (comparator(objects[start], objects[start + 1]))
                {
                    mLeft = objects[start];
                    mRight = objects[start + 1];
                }
                else
                {
                    mLeft = objects[start + 1];
                    mRight = objects[start];
                }
            }
            else
            {
                std::sort(objects.begin() + start, objects.begin() + end, comparator);
                auto mid = start + objectSpan / 2;
                mLeft = std::make_shared<BVHNode>(objects, start, mid);
                mRight = std::make_shared<BVHNode>(objects, mid, end);
            }            
            mBoundingBox = Scene::AABB(mLeft->BoundingBox(), mRight->BoundingBox());
        }
        BVHNode(const Scene::GeometryList& list) : BVHNode(list.mGeometries, 0, list.mGeometries.size()) {}

        bool Hit(const Scene::Ray& ray, Utilities::Interval rayT, Scene::HitPoint& hitPoint) const override
        {
            if (!mBoundingBox.Hit(ray, rayT))
            {
                return false;
            }
            bool hitLeft = mLeft->Hit(ray, rayT, hitPoint);
            bool hitRight = mRight->Hit(ray, rayT, hitPoint);
            return hitLeft || hitRight;
        }

        AABB BoundingBox() const override
        {
            return mBoundingBox;
        }
    
    private:
        static bool BoxCompare(const std::shared_ptr<Scene::Geometry> a, const std::shared_ptr<Scene::Geometry> b, int axisIndex)
        {
            return a->BoundingBox().Axis(axisIndex).mMin < b->BoundingBox().Axis(axisIndex).mMin;
        }
        static bool BoxCompareX(const std::shared_ptr<Scene::Geometry> a, const std::shared_ptr<Scene::Geometry> b)
        {
            return BoxCompare(a, b, 0);
        }
        static bool BoxCompareY(const std::shared_ptr<Scene::Geometry> a, const std::shared_ptr<Scene::Geometry> b)
        {
            return BoxCompare(a, b, 1);
        }
        static bool BoxCompareZ(const std::shared_ptr<Scene::Geometry> a, const std::shared_ptr<Scene::Geometry> b)
        {
            return BoxCompare(a, b, 2);
        }

    private:
        std::shared_ptr<Scene::Geometry> mLeft;
        std::shared_ptr<Scene::Geometry> mRight;
        Scene::AABB mBoundingBox;
    };
}