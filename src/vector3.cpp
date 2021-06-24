// MIT License

// Copyright (c) 2021 Florian Eigentler

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
#include <raytracing/vector3.h>

Vector3 operator+(const Vector3 &v, const Vector3 &w)
{
    Vector3 tmp{v};
    return tmp += w;
}

Vector3 operator-(const Vector3 &v)
{
    Vector3 tmp{v};
    return tmp *= -1;
}

Vector3 operator-(const Vector3 &v, const Vector3 &w)
{
    Vector3 tmp{v};
    return tmp -= w;
}

Vector3 &operator*=(const number_t t, Vector3 &v)
{
    return v *= t;
}

Vector3 operator*(const Vector3 &v, const number_t t)
{
    Vector3 tmp{v};
    return tmp *= t;
}

Vector3 operator*(const number_t t, const Vector3 &v)
{
    return v * t;
}

Vector3 operator*(const Vector3 &v, const Vector3 &w)
{
    Vector3 tmp{v};
    return tmp *= w;
}

Vector3 &operator/=(const number_t t, Vector3 &v)
{
    v[0] = t / v[0];
    v[1] = t / v[1];
    v[2] = t / v[2];
    return v;
}

Vector3 operator/(const Vector3 &v, const number_t t)
{
    Vector3 tmp{v};
    return tmp /= t;
}

Vector3 operator/(const number_t t, const Vector3 &v)
{
    Vector3 tmp{v};
    return t /= tmp;
}

Vector3 operator/(const Vector3 v, const Vector3 &w)
{
    Vector3 tmp{v};
    return tmp /= w;
}

Vector3 cross_Vector3(const Vector3 &u, const Vector3 &v)
{
    return Vector3{
        u[1] * v[2] - u[2] * v[1],
        u[2] * v[0] - u[0] * v[2],
        u[0] * v[1] - u[1] * v[0]};
}

Vector3 normalize_Vector3(const Vector3 &v)
{
    Vector3 tmp{v};
    return tmp.normalize();
}

Vector3 random_Vector3()
{
    return Vector3{random_number_t(), random_number_t(), random_number_t()};
}

Vector3 random_Vector3(number_t min, number_t max)
{
    return Vector3{random_number_t(min, max), random_number_t(min, max), random_number_t(min, max)};
}

Vector3 random_in_unit_disk()
{
    while (true)
    {
        Vector3 tmp{random_number_t(-1, 1), random_number_t(-1, 1), 0};
        if (tmp.length_squared() >= 1)
            continue;
        return tmp;
    }
}

Vector3 random_in_unit_sphere()
{
    while (true)
    {
        Vector3 tmp = random_Vector3(-1, 1);
        if (tmp.length_squared() >= 1)
            continue;
        return tmp;
    }
}

Vector3 norm_random_in_unit_sphere()
{
    return normalize_Vector3(random_in_unit_sphere());
}