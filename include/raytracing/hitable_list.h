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
#ifndef HITABLE_LIST_H
#define HITABLE_LIST_H

#include <memory>
#include <vector>
#include <raytracing/hitable.h>

typedef std::shared_ptr<Hitable> HitablePtr;

class HitableList : public Hitable
{
    std::vector<HitablePtr> objects;

public:
    HitableList() {}
    HitableList(HitablePtr object) { add(object); }

    void clear() { objects.clear(); }
    void add(HitablePtr object) { objects.push_back(object); }

    virtual bool hit(const Ray &r, number_t t_min, number_t t_max, HitRecord &rec) const override;
};

bool HitableList::hit(const Ray &r, number_t t_min, number_t t_max, HitRecord &rec) const
{
    HitRecord temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto &object : objects)
    {
        if (object->hit(r, t_min, closest_so_far, temp_rec))
        {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

#endif /* HITABLE_LIST_H */