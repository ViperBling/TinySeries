#include "Interval.hpp"

namespace Utilities
{
    const Interval Interval::Empty   (+Infinity, -Infinity);
    const Interval Interval::Universe(-Infinity, +Infinity);
}