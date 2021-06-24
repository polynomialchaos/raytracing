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