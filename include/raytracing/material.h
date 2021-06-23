#ifndef MATERIAL_H
#define MATERIAL_H

#include <memory>
#include <raytracing/raytracing.h>
#include <raytracing/hitable.h>
#include <raytracing/color.h>
#include <raytracing/ray.h>
#include <raytracing/texture.h>

typedef std::shared_ptr<Material> MaterialPtr;

class Material
{
public:
    virtual bool scatter(
        const Ray &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const = 0;
};

class Lambertian : public Material
{
    std::shared_ptr<Texture> _albedo;

public:
    Lambertian(const Color &albedo) : _albedo(std::make_shared<SolidColor>(albedo)) {}
    Lambertian(std::shared_ptr<Texture> texture) : _albedo(texture) {}

    virtual bool scatter(
        const Ray &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const override;
};

class Metal : public Material
{
    Color _albedo;
    number_t _fuzz;

public:
    Metal(const Color &albedo, const number_t fuzz) : _albedo(albedo), _fuzz(fuzz < 1 ? fuzz : 1) {}

    virtual bool scatter(
        const Ray &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const override;
};

class Dielectric : public Material
{
    number_t _ir; // Index of Refraction

public:
    Dielectric(number_t index_of_refraction) : _ir(index_of_refraction) {}

    virtual bool scatter(
        const Ray &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const override;
};

#endif /* MATERIAL_H */