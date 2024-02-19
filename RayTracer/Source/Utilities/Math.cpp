#include "Math.hpp"
#include "Utilities/Interval.hpp"

namespace Math
{
    void WriteColor(std::ostream& out, Color pixelColor, int samplesPerPixel)
    {
        auto r = pixelColor.x();
        auto g = pixelColor.y();
        auto b = pixelColor.z();

        auto scale = 1.0 / samplesPerPixel;
        r *= scale;
        g *= scale;
        b *= scale;
        static const Utilities::Interval intensity(0.0, 0.999);

        out << static_cast<int>(256 * intensity.Clamp(r)) << ' '
            << static_cast<int>(256 * intensity.Clamp(g)) << ' '
            << static_cast<int>(256 * intensity.Clamp(b)) << '\n';
    }
}