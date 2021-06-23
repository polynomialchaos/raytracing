#include <raytracing/vector3.h>
#include <raytracing/material.h>

Vector3 reflect(const Vector3 &v, const Vector3 &n)
{
    auto tmp = Vector3{v};
    return tmp - 2 * tmp.dot(n) * n;
}

Vector3 refract(const Vector3 &uv, const Vector3 &n, number_t etai_over_etat)
{
    auto cos_theta = fmin(-uv.dot(n), 1.0);
    Vector3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
    Vector3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}

static number_t reflectance(number_t cosine, number_t ref_idx)
{
    // Use Schlick's approximation for reflectance.
    auto r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}

bool Lambertian::scatter(
    const Ray &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const
{
    auto scatter_direction = rec.normal + norm_random_in_unit_sphere();

    // Catch degenerate scatter direction
    if (scatter_direction.is_near_zero())
        scatter_direction = rec.normal;

    scattered = Ray(rec.p, scatter_direction);
    attenuation = _albedo->value(rec.u, rec.v, rec.p);
    return true;
}

bool Metal::scatter(
    const Ray &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const
{
    Vector3 reflected = reflect(normalize_Vector3(r_in.direction()), rec.normal);
    // scattered = Ray(rec.p, reflected);
    scattered = Ray(rec.p, reflected + _fuzz * random_in_unit_sphere());
    attenuation = _albedo;
    return scattered.direction().dot(rec.normal) > 0;
}

bool Dielectric::scatter(
    const Ray &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const
{
    attenuation = Color{1};
    number_t refraction_ratio = rec.front_face ? (1.0 / _ir) : _ir;

    Vector3 unit_direction = normalize_Vector3(r_in.direction());
    number_t cos_theta = fmin(-unit_direction.dot(rec.normal), 1.0);
    number_t sin_theta = sqrt(1.0 - cos_theta * cos_theta);

    bool cannot_refract = refraction_ratio * sin_theta > 1.0;
    Vector3 direction;

    if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_number_t())
        direction = reflect(unit_direction, rec.normal);
    else
        direction = refract(unit_direction, rec.normal, refraction_ratio);

    scattered = Ray(rec.p, direction);
    return true;
}