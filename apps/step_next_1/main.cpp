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
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <omp.h>
#define PROJECT_FILE PROJECT ".ppm"
#define PROJECT_IMAGE "convert " PROJECT_FILE " " PROJECT ".png; rm " PROJECT_FILE

#include <raytracing/raytracing.h>
#include <raytracing/utils.h>
#include <raytracing/color.h>
#include <raytracing/hitable_list.h>
#include <raytracing/sphere.h>
#include <raytracing/camera.h>
#include <raytracing/material.h>
#include <raytracing/texture.h>

Vector3 random_in_hemisphere(const Vector3 &normal)
{
    Vector3 in_unit_sphere = random_in_unit_sphere();
    if (in_unit_sphere.dot(normal) > 0.0) // In the same hemisphere as the normal
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}

Color ray_color(const Ray &r, const Hitable &world, const int depth)
{
    HitRecord rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return Color{0};

    // if (world.hit(r, 0, infinity, rec))
    if (world.hit(r, 1e-3, infinity, rec))
    {
        Ray scattered;
        Color attenuation;
        if (rec.material->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth - 1);

        return Color{0};
    }

    Vector3 unit_direction = normalize_Vector3(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * Color{1} + t * Color{0.5, 0.7, 1};
}

class CheckerTexture : public Texture
{
    std::shared_ptr<Texture> _even;
    std::shared_ptr<Texture> _odd;

public:
    CheckerTexture(const std::shared_ptr<Texture> _even, const std::shared_ptr<Texture> _odd)
        : _even(_even), _odd(_odd) {}
    CheckerTexture(const Color &c1, const Color &c2)
        : _even(std::make_shared<SolidColor>(c1)), _odd(std::make_shared<SolidColor>(c2)) {}

    virtual Color value(double u, double v, const Point3 &p) const override
    {
        auto sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
        if (sines < 0)
            return _odd->value(u, v, p);
        else
            return _even->value(u, v, p);
    }
};

HitableList random_scene()
{
    HitableList world;

    auto checker = std::make_shared<CheckerTexture>(Color{0.2, 0.3, 0.1}, Color{0.9, 0.9, 0.9});
    world.add(std::make_shared<Sphere>(Point3{0, -1000, 0}, 1000, std::make_shared<Lambertian>(checker)));

    for (int a = -11; a < 11; ++a)
    {
        for (int b = -11; b < 11; ++b)
        {
            auto choose_mat = random_number_t();
            Point3 center{a + (number_t)0.9 * random_number_t(), (number_t)0.2, b + (number_t)0.9 * random_number_t()};

            if ((center - Point3(4, 0.2, 0)).length() > 0.9)
            {
                std::shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8)
                {
                    // diffuse
                    auto albedo = random_Vector3() * random_Vector3();
                    sphere_material = std::make_shared<Lambertian>(albedo);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95)
                {
                    // Metal
                    auto albedo = random_Vector3(0.5, 1);
                    auto fuzz = random_number_t(0, 0.5);
                    sphere_material = std::make_shared<Metal>(albedo, fuzz);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else
                {
                    // glass
                    sphere_material = std::make_shared<Dielectric>(1.5);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = std::make_shared<Dielectric>(1.5);
    world.add(std::make_shared<Sphere>(Point3{0, 1, 0}, 1.0, material1));

    auto material2 = std::make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    world.add(std::make_shared<Sphere>(Point3{-4, 1, 0}, 1.0, material2));

    auto material3 = std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    world.add(std::make_shared<Sphere>(Point3{4, 1, 0}, 1.0, material3));

    return world;
}

int main(int argc, char const *argv[])
{
    // Image
    const auto aspect_ratio = ASPECT_RATIO;
    const int image_width = IMAGE_WIDTH;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = SAMPLES_PER_PIXEL;
    const int max_depth = MAX_DEPTH;

    // World
    HitableList world = random_scene();

    // Camera
    Point3 lookfrom{13, 2, 3};
    Point3 lookat{0, 0, 0};
    Vector3 vup{0, 1, 0};
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;

    Camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

    // Render
    Color *pixel = (Color *)malloc(sizeof(Color) * image_width * image_height);
    int row_counter = image_height;

#pragma omp parallel for
    for (int j = image_height - 1; j >= 0; --j)
    {
        --row_counter;
        std::cout << (ParallelStream() << "\rScanlines remaining: " << row_counter << ' ').toString() << std::flush;

        for (int i = 0; i < image_width; ++i)
        {
            pixel[j * image_width + i] = Color{0, 0, 0};

            for (int s = 0; s < samples_per_pixel; ++s)
            {
                auto u = (i + random_number_t()) / (image_width - 1);
                auto v = (j + random_number_t()) / (image_height - 1);
                Ray r = cam.get_ray(u, v);
                pixel[j * image_width + i] += ray_color(r, world, max_depth);
            }
        }
    }

    // Output
    std::ofstream file;
    file.open(PROJECT_FILE);
    file << "P3\n"
         << image_width << " " << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j)
        for (int i = 0; i < image_width; ++i)
            write_color(file, pixel[j * image_width + i], samples_per_pixel, 2);

    file << "\n";
    file.close();
    system(PROJECT_IMAGE);

    // Finalize
    free(pixel);

    std::cout << "\nDone.\n";
    return 0;
}