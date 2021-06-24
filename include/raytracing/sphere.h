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
#ifndef SPHERE_H
#define SPHERE_H

#include <raytracing/raytracing.h>
#include <raytracing/hitable.h>
#include <raytracing/material.h>

class Sphere : public Hitable
{
    Point3 _center;
    number_t _radius;
    MaterialPtr _material;
    void get_sphere_uv(const Point3 &p, double &u, double &v) const;

public:
    Sphere() {}
    Sphere(Point3 center, number_t radius)
        : _center(center), _radius(radius), _material(NULL){};
    Sphere(Point3 center, number_t radius, MaterialPtr material)
        : _center(center), _radius(radius), _material(material){};

    virtual bool hit(const Ray &r, number_t t_min, number_t t_max, HitRecord &rec) const override;
};

bool Sphere::hit(const Ray &r, number_t t_min, number_t t_max, HitRecord &rec) const
{
    Vector3 oc = r.origin() - _center;
    auto a = r.direction().length_squared();
    auto half_b = oc.dot(r.direction());
    auto c = oc.length_squared() - _radius * _radius;

    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0)
        return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root)
    {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);

    Vector3 outward_normal = (rec.p - _center) / _radius;
    rec.set_face_normal(r, outward_normal);
    rec.material = _material;
    get_sphere_uv(outward_normal, rec.u, rec.v);

    return true;
}

void Sphere::get_sphere_uv(const Point3 &p, double &u, double &v) const
{
    // p: a given point on the sphere of radius one, centered at the origin.
    // u: returned value [0,1] of angle around the Y axis from X=-1.
    // v: returned value [0,1] of angle from Y=-1 to Y=+1.
    //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
    //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
    //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

    auto theta = acos(-p.y());
    auto phi = atan2(-p.z(), p.x()) + pi;

    u = phi / (2 * pi);
    v = theta / pi;
}

#endif /* SPHERE_H */