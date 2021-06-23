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