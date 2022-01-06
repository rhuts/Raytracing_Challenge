#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <memory>
#include <vector>

#include "Hittable.h"


using std::shared_ptr;
using std::make_shared;

class HittableList : public Hittable
{
public:
    HittableList() {}
    HittableList(shared_ptr<Hittable> object) { add(object); }

    void clear() { objects.clear(); }
    void add(shared_ptr<Hittable> object) { objects.push_back(object); }
    
    virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const override;

    std::vector<shared_ptr<Hittable>> objects;
};

bool HittableList::hit(const Ray& r, double t_min, double t_max, hit_record& rec) const
{
    hit_record temp_rec;
    bool is_hit = false;
    auto closest_so_far = t_max;

    for (const auto& object : objects)
    {
        if (object->hit(r, t_min, closest_so_far, temp_rec))
        {
            is_hit = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return is_hit;
}

#endif