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