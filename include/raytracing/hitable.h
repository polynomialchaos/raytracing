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
#ifndef HITABLE_H
#define HITABLE_H

#include <raytracing/raytracing.h>
#include <raytracing/ray.h>

class Material;

struct HitRecord
{
    Point3 p;
    Vector3 normal;
    number_t t;
    std::shared_ptr<Material> material;
    bool front_face;
    double u;
    double v;

    inline void set_face_normal(const Ray &r, const Vector3 &outward_normal)
    {
        front_face = r.direction().dot(outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hitable
{
public:
    virtual bool hit(const Ray &r, number_t t_min, number_t t_max, HitRecord &rec) const = 0;
};

#endif /* HITABLE_H */