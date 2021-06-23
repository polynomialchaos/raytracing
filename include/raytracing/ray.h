#ifndef RAY_H
#define RAY_H

#include <raytracing/raytracing.h>
#include <raytracing/vector3.h>

class Ray
{
    Point3 _origin;
    Vector3 _direction;

public:
    Ray() {}
    Ray(const Point3 &origin, const Vector3 &direction)
        : _origin(origin), _direction(direction) {}

    Point3 origin() const { return _origin; }
    Vector3 direction() const { return _direction; }
    Point3 at(number_t t) const { return _origin + t * _direction; }
};

#endif /* RAY_H */