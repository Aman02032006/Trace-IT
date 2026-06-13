#ifndef SCENE_H
#define SCENE_H

#pragma once
#include "rtweekend.h"
#include "sphere.h"
#include <vector>
#include <memory>

using namespace std;

class scene : public hittable
{
public:
    vector<shared_ptr<sphere>> spheres;

    // Constructors
    scene() {}
    scene(shared_ptr<sphere> sphere) { add(sphere); }

    // Scene management
    void clear() { spheres.clear(); }
    void add(shared_ptr<sphere> object) { spheres.push_back(object); }

    // Ray intersection
    bool hit(const ray &r, interval ray_t, hit_record &rec) const override
    {
        hit_record temp_rec;
        bool hit_anything = false;
        auto closest_so_far = ray_t.max;

        for (const auto &sphere : spheres)
        {
            if (sphere->hit(r, interval(ray_t.min, closest_so_far), temp_rec))
            {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }
        return hit_anything;
    }
};

#endif
