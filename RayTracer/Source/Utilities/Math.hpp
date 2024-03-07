#pragma once

#include "Utilities.hpp"

#include <cmath>
#include <iostream>

namespace Math
{
    class Vector3
    {
    public:
        Vector3() : data{0, 0, 0} {}
        Vector3(double x, double y, double z) : data{x, y, z} {}

        double x() const { return data[0]; }
        double y() const { return data[1]; }
        double z() const { return data[2]; }

        Vector3 operator-() const { return Vector3(-data[0], -data[1], -data[2]); }
        double operator[](int i) const { return data[i]; }
        double& operator[](int i) { return data[i]; }

        Vector3& operator+=(const Vector3& v)
        {
            data[0] += v.data[0];
            data[1] += v.data[1];
            data[2] += v.data[2];
            return *this;
        }

        Vector3& operator*=(const double t)
        {
            data[0] *= t;
            data[1] *= t;
            data[2] *= t;
            return *this;
        }

        Vector3& operator/=(const double t)
        {
            return *this *= 1 / t;
        }

        double Length() const
        {
            return std::sqrt(LengthSquared());
        }

        double LengthSquared() const
        {
            return data[0] * data[0] + data[1] * data[1] + data[2] * data[2];
        }

        bool NearZero() const
        {
            const auto s = 1e-8;
            return (std::fabs(data[0]) < s) && (std::fabs(data[1]) < s) && (std::fabs(data[2]) < s);
        }

        static Vector3 Random()
        {
            return Vector3(Utilities::RandomDouble(), Utilities::RandomDouble(), Utilities::RandomDouble());
        }

        static Vector3 Random(double min, double max)
        {
            return Vector3(Utilities::RandomDouble(min, max), Utilities::RandomDouble(min, max), Utilities::RandomDouble(min, max));
        }

    public:
        double data[3];
    };

    using Point3 = Vector3;
    using Color = Vector3;

    inline std::ostream& operator<<(std::ostream& out, const Vector3& v)
    {
        return out << v.x() << ' ' << v.y() << ' ' << v.z();
    }

    inline Vector3 operator+(const Vector3& u, const Vector3& v)
    {
        return Vector3(u.x() + v.x(), u.y() + v.y(), u.z() + v.z());
    }

    inline Vector3 operator-(const Vector3& u, const Vector3& v)
    {
        return Vector3(u.x() - v.x(), u.y() - v.y(), u.z() - v.z());
    }

    inline Vector3 operator*(const Vector3& u, const Vector3& v)
    {
        return Vector3(u.x() * v.x(), u.y() * v.y(), u.z() * v.z());
    }

    inline Vector3 operator*(double t, const Vector3& v)
    {
        return Vector3(t * v.x(), t * v.y(), t * v.z());
    }

    inline Vector3 operator*(const Vector3& v, double t)
    {
        return t * v;
    }

    inline Vector3 operator/(Vector3 v, double t)
    {
        return (1 / t) * v;
    }

    inline double Dot(const Vector3& u, const Vector3& v)
    {
        return u.x() * v.x() + u.y() * v.y() + u.z() * v.z();
    }

    inline Vector3 Cross(const Vector3& u, const Vector3& v)
    {
        return Vector3(u.y() * v.z() - u.z() * v.y(),
                       u.z() * v.x() - u.x() * v.z(),
                       u.x() * v.y() - u.y() * v.x());
    }

    inline Vector3 Normalize(Vector3 v)
    {
        return v / v.Length();
    }

    inline Vector3 RandomInUnitDisk()
    {
        while (true)
        {
            auto p = Vector3(Utilities::RandomDouble(-1, 1), Utilities::RandomDouble(-1, 1), 0);
            if (p.LengthSquared() < 1)
            {
                return p;
            }
        }
    }

    inline Vector3 RandomOnUnitSphere()
    {
        while (true)
        {
            auto p = Vector3::Random(-1, 1);
            if (p.LengthSquared() < 1)
            {
                return p;
            }
        }
    }

    inline Vector3 RandomUnitVector()
    {
        return Normalize(RandomOnUnitSphere());
    }

    inline Vector3 RandomOnHemisphere(const Vector3& normal)
    {
        Vector3 onUnitSphere = RandomOnUnitSphere();
        if (Dot(onUnitSphere, normal) > 0.0)
        {
            return onUnitSphere;
        }
        else
        {
            return -onUnitSphere;
        }
    }

    inline int RandomInt(int min, int max)
    {
        return static_cast<int>(Utilities::RandomDouble(min, max + 1));
    }

    inline Vector3 Reflect(const Vector3& v, const Vector3& n)
    {
        return v - 2 * Dot(v, n) * n;
    }

    inline Vector3 Refract(const Vector3& uv, const Vector3& n, double etaiOverEtat)
    {
        auto cosTheta = fmin(Dot(-uv, n), 1.0);
        Vector3 rOutPerp = etaiOverEtat * (uv + cosTheta * n);
        Vector3 rOutParallel = -std::sqrt(fabs(1.0 - rOutPerp.LengthSquared())) * n;
        return rOutParallel + rOutPerp;
    }

    inline double LinearToGamma(double linear)
    {
        return std::sqrt(linear);
    }

    void WriteColor(std::ostream& out, Color pixelColor, int samplesPerPixel);
}