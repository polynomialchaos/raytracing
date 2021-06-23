#ifndef TEXTURE_H
#define TEXTURE_H

#include "raytracing/color.h"

class Texture
{
public:
    virtual Color value(double u, double v, const Point3 &p) const = 0;
};

class SolidColor : public Texture
{
    Color _color;

public:
    SolidColor() {}
    SolidColor(const Color &c) : _color{c} {}

    virtual Color value(double u, double v, const Point3 &p) const override
    {
        return _color;
    }
};

#endif /* TEXTURE_H */