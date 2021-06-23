#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <omp.h>
#define PROJECT_FILE PROJECT".ppm"
#define PROJECT_IMAGE "convert " PROJECT_FILE " " PROJECT ".png; rm " PROJECT_FILE

#include <raytracing/raytracing.h>
#include <raytracing/utils.h>
#include <raytracing/color.h>
#include <raytracing/hitable_list.h>
#include <raytracing/sphere.h>
#include <raytracing/camera.h>

Vector3 random_in_hemisphere(const Vector3 &normal) {
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
        // Point3 target = rec.p + rec.normal + random_in_unit_sphere();
        // Point3 target = rec.p + rec.normal + norm_random_in_unit_sphere();
        Point3 target = rec.p + random_in_hemisphere(rec.normal);
        return 0.5 * ray_color(Ray(rec.p, target - rec.p), world, depth - 1);
    }

    Vector3 unit_direction = normalize_Vector3(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * Color{1} + t * Color{0.5, 0.7, 1};
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
    HitableList world;
    world.add(std::make_shared<Sphere>(Point3{0, 0, -1}, 0.5));
    world.add(std::make_shared<Sphere>(Point3{0, -100.5, -1}, 100));

    // Camera
    Camera cam{};

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
    system( PROJECT_IMAGE );

    // Finalize
    free(pixel);

    std::cout << "\nDone.\n";
    return 0;
}