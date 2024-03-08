#include "Interval.hpp"

namespace Utilities
{
    const Interval Interval::Empty   (+Math::Infinity, -Math::Infinity);
    const Interval Interval::Universe(-Math::Infinity, +Math::Infinity);
}