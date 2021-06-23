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

bool hit_sphere(const Point3 &center, const double radius, const Ray &r) {
    Vector3 oc = r.origin() - center;
    auto a = r.direction().dot( r.direction() );
    auto b = 2.0 * oc.dot(r.direction());
    auto c = oc.dot(oc) - radius*radius;
    auto discriminant = b*b - 4*a*c;
    return (discriminant > 0);
}

Color ray_color(const Ray &r)
{
    if (hit_sphere(Point3{0,0,-1}, 0.5, r))
        return Color{1, 0, 0};

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

    // Camera
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = Point3{0};
    auto horizontal = Vector3{viewport_width, 0, 0};
    auto vertical = Vector3{0, viewport_height, 0};
    auto lower_left_corner = origin - horizontal / 2 - vertical / 2 - Vector3{0, 0, focal_length};

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
            auto u = double(i) / (image_width - 1);
            auto v = double(j) / (image_height - 1);
            Ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);
            pixel[j * image_width + i] = ray_color(r);
        }
    }

    // Output
    std::ofstream file;
    file.open(PROJECT_FILE);
    file << "P3\n"
         << image_width << " " << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j)
        for (int i = 0; i < image_width; ++i)
            write_color(file, pixel[j * image_width + i], 1, 1);

    file << "\n";
    file.close();
    system( PROJECT_IMAGE );

    // Finalize
    free(pixel);

    std::cout << "\nDone.\n";
    return 0;
}