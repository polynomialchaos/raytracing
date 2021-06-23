#include <raytracing/color.h>

number_t clamp(number_t x, number_t min, number_t max)
{
    if (x < min)
        return min;
    if (x > max)
        return max;
    return x;
}

void write_color(std::ostream &out, const Color color,
                 const int samples_per_pixel, const number_t gamma)
{
    // Divide the color by the number of samples and gamma-correct for gamma=2.0.
    number_t scale = 1.0 / samples_per_pixel;
    number_t sgamma = 1.0 / gamma;
    number_t r = pow(scale * color.r(), sgamma);
    number_t g = pow(scale * color.g(), sgamma);
    number_t b = pow(scale * color.b(), sgamma);

    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}