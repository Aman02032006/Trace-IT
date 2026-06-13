#ifndef INTERVAL_H
#define INTERVAL_H

#pragma once
#include "rtweekend.h"

class interval
{
public:
    double min, max;

    // Constructors
    interval() : min(infinity), max(-infinity) {}
    interval(double min_val, double max_val) : min(min_val), max(max_val) {}

    // Member functions
    double size() const
    {
        return max - min;
    }

    bool contains(double x) const
    {
        return min <= x && x <= max;
    }

    bool surrounds(double x) const
    {
        return min < x && x < max;
    }

    double clamp(double x) const
    {
        if (x < min)
            return min;
        if (x > max)
            return max;
        return x;
    }

    // Static members
    static const interval empty;
    static const interval universe;
};

// Static member definitions
const interval interval::empty(infinity, -infinity);
const interval interval::universe(-infinity, infinity);

#endif
