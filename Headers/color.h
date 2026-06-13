# ifndef COLOR_H
# define COLOR_H

# include "vec3.h"
# include "interval.h"
# include <iostream>
# include <map>
# include <random>

using color = vec3;

inline double linear_to_gamma(double linear_component)
{
    if (linear_component > 0)
        return std::sqrt(linear_component);
    return 0;
}

inline void write_color(std::ostream &out, const color &pixel_color)
{
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();
    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);
    static const interval intensity(0.000, 0.999);
    int rbyte = int(256 * intensity.clamp(r));
    int gbyte = int(256 * intensity.clamp(g));
    int bbyte = int(256 * intensity.clamp(b));
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

inline color RandomColor()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> dist(0.0, 1.0);
    return color(dist(gen), dist(gen), dist(gen));
}

std::map<std::string, color> named_colors = {
    {"red", color(1, 0, 0)},
    {"green", color(0, 1, 0)},
    {"blue", color(0, 0, 1)},
    {"yellow", color(1.0, 1.0, 0.0)},
    {"cyan", color(0.0, 1.0, 1.0)},
    {"magenta", color(204.0/255, 0.0, 204.0/255)},
    {"white", color(1.0, 1.0, 1.0)},
    {"black", color(0.0, 0.0, 0.0)}
};

#endif // COLOR_H
