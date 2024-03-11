#pragma once

#include "Math.hpp"

namespace Utilities
{
    // 区间类，代表一个区间（一维）
    class Interval
    {
    public:
        Interval() 
            : mMin(+Math::Infinity)
            , mMax(-Math::Infinity)
        {}
        Interval(double min, double max)
            : mMin(min)
            , mMax(max)
        {}
        Interval(const Interval& A, const Interval& B)
            : mMin(std::fmin(A.mMin, B.mMin))
            , mMax(std::fmax(A.mMax, B.mMax))
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

    inline Interval operator+(const Interval& iVal, double offset)
    {
        return Interval(iVal.mMin + offset, iVal.mMax + offset);
    }

    inline Interval operator+(double offset, const Interval& iVal)
    {
        return iVal + offset;
    }
}
