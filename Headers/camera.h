#pragma once

#include "hittable.h"
#include "vec3.h"
#include "ray.h"
#include "color.h"
#include "material.h"
#include <fstream>
#include <thread>
#include <vector>
#include <algorithm>
#include <omp.h>

using namespace std;

class camera
{
public:
    double aspect_ratio = 1.0;
    int image_width = 100;
    int samples_per_pixel = 10;
    int max_depth = 10;

    double vfov = 90;                  // Vertical View angle (field of view)
    point3 lookfrom = point3(0, 0, 0); // Point Camera is looking from
    point3 lookat = point3(0, 0, -1);  // Point Camera is looking at
    vec3 vup = vec3(0, 1, 0);          // Camera - relative "up" direction

    void render(const hittable &world, const std::string &filename) // Renders the frame using multithreading and writes to filename.ppm
    {
        initialize();

        std::ofstream out(filename);
        out << "P3\n"
            << image_width << ' ' << image_height << "\n255\n";

        vector<vector<color>> framebuffer(image_height, vector<color>(image_width));

        #pragma omp parallel for schedule(dynamic)
        for (int i = 0; i < image_height; i++)
        {
            for (int j = 0; j < image_width; j++)
            {
                color pixel_color(0, 0, 0);
                for (int sample = 0; sample < samples_per_pixel; sample++)
                {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }
                framebuffer[i][j] = pixel_color;
            }
        }

        for (int i = 0; i < image_height; ++i)
        {
            std::clog << "\rScanlines remaining: " << (image_height - i) << ' ' << std::flush;
            for (int j = 0; j < image_width; ++j)
            {
                write_color(out, pixel_samples_scale * framebuffer[i][j]);
            }
        }

        std::clog << "\rDone.                               \n";
    }

private:
    int image_height;
    double pixel_samples_scale;
    point3 center;
    point3 pixel00_loc;
    vec3 pixel_delta_u;
    vec3 pixel_delta_v;
    vec3 u, v, w;

    void initialize()
    {
        image_height = static_cast<int>(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        pixel_samples_scale = 1.0 / samples_per_pixel;

        center = lookfrom;

        // Viewport Dimensions
        auto focal_length = (lookfrom - lookat).length();
        auto theta = degrees_to_radians(vfov);
        auto h = std::tan(theta / 2);
        auto viewport_height = 2.0 * h * focal_length;
        auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);

        // Calculate the u, v, w unit basis vectors for the camera frame
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        // Calculate the viewport vectors across the horizontal and down the vertical viewport edges
        auto viewport_u = viewport_width * u;
        auto viewport_v = -viewport_height * v;

        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        auto viewport_upper_left = center - (focal_length * w) - viewport_u / 2 - viewport_v / 2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }

    ray get_ray(int i, int j) const
    {
        auto offset = sample_square();
        auto pixel_sample = pixel00_loc + (i + offset.y()) * pixel_delta_v + (j + offset.x()) * pixel_delta_u;
        return ray(center, pixel_sample - center);
    }

    vec3 sample_square() const
    {
        return vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }

    color ray_color(const ray &r, int depth, const hittable &world)
    {
        if (depth <= 0)
            return color(0, 0, 0);

        hit_record rec;

        if (world.hit(r, interval(0.001, infinity), rec))
        {
            ray scattered;
            color attenuation;

            if (rec.mat->scatter(r, rec, attenuation, scattered))
                return attenuation * ray_color(scattered, depth - 1, world);
            else
                return rec.mat->emitted(r, rec);
        }

        vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - a) * color(1.0, 0.5, 0.2) + a * color(0.2, 0.1, 0.4);
    }
};
