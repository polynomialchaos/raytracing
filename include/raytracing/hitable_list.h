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