#ifndef RAY_H
#define RAY_H

#pragma once
#include "vec3.h"

class ray
{
private:
    point3 orig;
    vec3 dir;

public:
    // Constructors
    ray() {} // Default constructor
    ray(const point3 &origin, const vec3 &direction) : orig(origin), dir(direction) {}

    // Accessors
    const point3 &origin() const { return orig; }
    const vec3 &direction() const { return dir; }

    // Ray position calculation
    point3 at(double t) const
    {
        return orig + t * dir;
    }
};

#endif
