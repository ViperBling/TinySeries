#pragma once

#include "Utilities.hpp"

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

public:
    double mMin;
    double mMax;
    static const Interval Empty;
    static const Interval Universe;
};

const Interval Interval::Empty   (+Infinity, -Infinity);
const Interval Interval::Universe(-Infinity, +Infinity);