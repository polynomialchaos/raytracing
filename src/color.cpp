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