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
#ifndef COLOR_H
#define COLOR_H

#include <iostream>
#include <raytracing/raytracing.h>
#include <raytracing/vector3.h>

class Color : public Vector3
{
    using Vector3::dot;
    using Vector3::is_near_zero;
    using Vector3::length;
    using Vector3::length_squared;
    using Vector3::normalize;
    using Vector3::sum;
    using Vector3::x;
    using Vector3::y;
    using Vector3::z;

public:
    Color() : Vector3{0, 0, 0} {}
    Color(const number_t c) : Vector3{c, c, c} {}
    Color(const Color &c) : Vector3{c.r(), c.g(), c.b()} {}
    Color(const Vector3 &v) : Vector3{v} {}
    Color(const number_t r, const number_t g, const number_t b) : Vector3{r, g, b} {}
    number_t r() const { return x(); }
    number_t g() const { return y(); }
    number_t b() const { return z(); }
};

inline std::ostream &operator<<(std::ostream &out, const Color &c)
{
    return out << "rgb(" << c.r() << ',' << c.g() << ',' << c.b() << ')';
}

void write_color(std::ostream &out, const Color color,
                 const int samples_per_pixel = 1, const number_t gamma = 2.0);

#endif /* COLOR_H */