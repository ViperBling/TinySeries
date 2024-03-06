#pragma once

#include "Utilities.hpp"

namespace Utilities
{
    class Interval
    {
    public:
        Interval() 
            : mMin(+Infinity)
            , mMax(-Infinity)
        {}
        Interval(double min, double max)
            : mMin(min)
            , mMax(max)
        {}
        bool Contains(double value) const
        {
            return value >= mMin && value <= mMax;
        }
        bool Contains(const Interval& other) const
        {
            return mMin <= other.mMin && mMax >= other.mMax;
        }

        bool Surrounds(double value) const
        {
            return value > mMin && value < mMax;
        }

        double Clamp(double value) const
        {
            return std::clamp(value, mMin, mMax);
        }

        double Size() const
        {
            return mMax - mMin;
        }

        Interval Expand(double delta) const
        {
            auto padding = delta * 0.5;
            return Interval(mMin - padding, mMax + padding);
        }

    public:
        double mMin;
        double mMax;
        static const Interval Empty;
        static const Interval Universe;
    };
}
