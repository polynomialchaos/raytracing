#ifndef CAMERA_H
#define CAMERA_H

#include <raytracing/raytracing.h>
#include <raytracing/vector3.h>
#include <raytracing/ray.h>
#include <raytracing/utils.h>

class Camera
{
    Point3 origin;
    Point3 lower_left_corner;
    Vector3 horizontal;
    Vector3 vertical;
    Vector3 u, v, w;
    number_t lens_radius;

public:
    Camera()
    {
        auto aspect_ratio = 16.0 / 9.0;
        auto viewport_height = 2.0;
        auto viewport_width = aspect_ratio * viewport_height;
        auto focal_length = 1.0;

        origin = Point3{0};
        horizontal = Vector3{viewport_width, 0.0, 0.0};
        vertical = Vector3{0.0, viewport_height, 0.0};
        lower_left_corner = origin - horizontal / 2 - vertical / 2 - Vector3{0, 0, focal_length};
    }
    Camera(
        Point3 lookfrom,
        Point3 lookat,
        Vector3 vup,
        number_t vfov, // vertical field-of-view in degrees
        number_t aspect_ratio,
        number_t aperture,
        number_t focus_dist)
    {
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta / 2);
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;

        w = normalize_Vector3(lookfrom - lookat);
        u = normalize_Vector3(cross_Vector3(vup, w));
        v = cross_Vector3(w, u);

        origin = lookfrom;
        horizontal = focus_dist * viewport_width * u;
        vertical = focus_dist * viewport_height * v;
        lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_dist * w;

        lens_radius = aperture / 2;
    }

    Ray get_ray(number_t s, number_t t) const
    {
        Vector3 rd = lens_radius * random_in_unit_disk();
        Vector3 offset = u * rd.x() + v * rd.y();

        return Ray(
            origin + offset,
            lower_left_corner + s * horizontal + t * vertical - origin - offset);

        // return Ray(origin, lower_left_corner + s * horizontal + t * vertical - origin);
    }
};

#endif /* CAMERA_H */