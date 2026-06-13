#include "rtweekend.h"
#include "camera.h"
#include "scene.h"
#include "color.h"
#include "material.h"
#include "sphere.h"
#include <sstream>
#include <iomanip>

using namespace std;

// helper function to return a point rotating in a plane which is also rotating
point3 rotating_point(double theta, double phi)
{
    return point3(cos(theta) * cos(phi), cos(theta) * sin(phi), sin(theta));
}

scene make_scene(int i, int number_of_frames)
{
    // material definitions
    auto glass = make_shared<dielectric>(1.50);
    auto bubble = make_shared<dielectric>(1.0 / 1.50);
    auto core = make_shared<metal>(color(0.95, 0.95, 1.0), 0.01);
    auto redstone = make_shared<metal>(color(153 / 255.0, 0, 0), 0.1);
    auto bluestone = make_shared<metal>(color(0, 0, 170 / 255.0), 0.1);
    auto greenstone = make_shared<metal>(color(0, 140 / 255.0, 0), 0.1);
    auto purplestone = make_shared<metal>(color(153.0 / 255, 51.0 / 255, 1), 0.1);
    auto goldstone = make_shared<metal>(color(219.0 / 255, 172.0 / 255, 52.0 / 255), 0.1);
    auto orangestone = make_shared<metal>(color(255.0 / 255, 164.0 / 255, 0), 0.1);
    auto ground = make_shared<lambertian>(color(0.8, 0.8, 0.1));

    auto core_radius = 1.0;
    auto stone_radius = 0.5;

    // Creating the scene
    scene world;

    // Rotation Parameters
    auto theta = 20 * i * pi / number_of_frames; // object rotation
    auto phi = i * pi / number_of_frames;        // object rotation

    world.add(make_shared<sphere>(point3(0, 0, 0), 10.0, glass));
    world.add(make_shared<sphere>(point3(0, 0, 0), 9.8, bubble));
    world.add(make_shared<sphere>(point3(0, -400, 0), 395.0, ground));
    world.add(make_shared<sphere>(point3(0, 0, 0), 1.0, core));
    world.add(make_shared<sphere>(5 * rotating_point(theta, phi), stone_radius, bluestone));
    world.add(make_shared<sphere>(-5 * rotating_point(theta, phi), stone_radius, purplestone));
    world.add(make_shared<sphere>(5 * rotating_point(theta + pi / 3, phi + 2 * pi / 3), stone_radius, redstone));
    world.add(make_shared<sphere>(-5 * rotating_point(theta + pi / 3, phi + 2 * pi / 3), stone_radius, greenstone));
    world.add(make_shared<sphere>(5 * rotating_point(theta + 2 * pi / 3, phi + 4 * pi / 3), stone_radius, goldstone));
    world.add(make_shared<sphere>(-5 * rotating_point(theta + 2 * pi / 3, phi + 4 * pi / 3), stone_radius, orangestone));

    return world;
}

int main()
{
    // Defining the camera and parameters which remain constant throughout the animation
    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 1280;
    cam.samples_per_pixel = 100;
    cam.max_depth = 25;
    cam.lookat = point3(0, 1, 0);
    cam.vup = vec3(0, 1, 0);
    cam.vfov = 60;

    // variable number of frames
    int number_of_frames = 300;

    // loop which updates the scene
    for (int i = 0; i < number_of_frames; i++)
    {
        scene world = make_scene(i, number_of_frames);

        double r = 15 - i * 8.0 / number_of_frames;
        cam.lookfrom = r * point3(1, 0.5, 1);

        ostringstream filename_stream;
        filename_stream << "frame" << setfill('0') << setw(3) << i << ".ppm";
        string filename = filename_stream.str();

        cam.render(world, filename);
    }

    std::cout << "Rendering video using FFmpeg...\n";

    // FFmpeg command to encode into MP4 without scaling
    std::string ffmpeg_cmd =
        "ffmpeg -framerate 30 -i frame%03d.ppm "
        "-c:v libx264 -pix_fmt yuv420p out.mp4";

    // Run FFmpeg
    int result = system(ffmpeg_cmd.c_str());

    if (result != 0)
    {
        std::cerr << "FFmpeg failed to create the video.\n";
    }
    else
    {
        std::cout << "Video successfully created: out.mp4\n";
    }
}