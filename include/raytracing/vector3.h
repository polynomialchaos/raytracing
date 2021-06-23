#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>
#include <iostream>
#include <raytracing/raytracing.h>
#include <raytracing/utils.h>

class Vector3
{
    number_t _v[3];

public:
    Vector3() : _v{0, 0, 0} {}
    Vector3(const number_t v) : _v{v, v, v} {}
    Vector3(const Vector3 &v) : _v{v._v[0], v._v[1], v._v[2]} {}
    Vector3(const number_t v0, const number_t v1, const number_t v2) : _v{v0, v1, v2} {}

    number_t dot(const Vector3 &v) const
    {
        return _v[0] * v._v[0] + _v[1] * v._v[1] + _v[2] * v._v[2];
    }

    bool is_near_zero() const
    {
        const number_t s = 1e-8;
        return (fabs(_v[0]) < s) && (fabs(_v[1]) < s) && (fabs(_v[2]) < s);
    }

    number_t length() const { return std::sqrt(length_squared()); }
    number_t length_squared() const { return _v[0] * _v[0] + _v[1] * _v[1] + _v[2] * _v[2]; }

    Vector3 &normalize()
    {
        return *this /= length();
    }

    number_t sum() const { return _v[0] + _v[1] + _v[2]; }

    number_t x() const { return _v[0]; }
    number_t y() const { return _v[1]; }
    number_t z() const { return _v[2]; }

    number_t operator[](int i) const { return _v[i]; }
    number_t &operator[](int i) { return _v[i]; }

    Vector3 &operator+=(const Vector3 &v)
    {
        _v[0] += v._v[0];
        _v[1] += v._v[1];
        _v[2] += v._v[2];
        return *this;
    }

    Vector3 &operator-=(const Vector3 &v)
    {
        _v[0] -= v._v[0];
        _v[1] -= v._v[1];
        _v[2] -= v._v[2];
        return *this;
    }

    Vector3 &operator*=(const number_t t)
    {
        _v[0] *= t;
        _v[1] *= t;
        _v[2] *= t;
        return *this;
    }

    Vector3 &operator*=(const Vector3 &v)
    {
        _v[0] *= v[0];
        _v[1] *= v[1];
        _v[2] *= v[2];
        return *this;
    }

    Vector3 &operator/=(const number_t t)
    {
        _v[0] /= t;
        _v[1] /= t;
        _v[2] /= t;
        return *this;
    }

    Vector3 &operator/=(const Vector3 &v)
    {
        _v[0] /= v[0];
        _v[1] /= v[1];
        _v[2] /= v[2];
        return *this;
    }
};

inline std::ostream &operator<<(std::ostream &out, const Vector3 &v)
{
    return out << '[' << v.x() << ',' << v.y() << ',' << v.z() << ']';
}

Vector3 operator+(const Vector3 &v, const Vector3 &w);
Vector3 operator-(const Vector3 &v);
Vector3 operator-(const Vector3 &v, const Vector3 &w);
Vector3 &operator*=(const number_t t, Vector3 &v);
Vector3 operator*(const Vector3 &v, const number_t t);
Vector3 operator*(const number_t t, const Vector3 &v);
Vector3 operator*(const Vector3 &v, const Vector3 &w);
Vector3 &operator/=(const number_t t, Vector3 &v);
Vector3 operator/(const Vector3 &v, const number_t t);
Vector3 operator/(const number_t t, const Vector3 &v);
Vector3 operator/(const Vector3 v, const Vector3 &w);
Vector3 cross_Vector3(const Vector3 &u, const Vector3 &v);
Vector3 normalize_Vector3(const Vector3 &v);
Vector3 random_Vector3();
Vector3 random_Vector3(number_t min, number_t max);

Vector3 random_in_unit_disk();
Vector3 random_in_unit_sphere();
Vector3 norm_random_in_unit_sphere();

using Point3 = Vector3;

#endif /* VECTOR_H */