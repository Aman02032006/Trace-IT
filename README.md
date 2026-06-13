# Trace It - C++ Ray Tracer

A multi-threaded CPU-based 3D ray tracer written in C++ from scratch. This project is heavily inspired by the famous *"Ray Tracing in One Weekend"* series but has been extended to support **animation generation** and **multi-threading**. It can render both high-quality static images and dynamic, animated scenes encoded into `.mp4` video files.

## Features

- **Physics Based Path Tracing:** Accurately simulates the bouncing of light to create realistic shadows, reflections, and refractions.
- **Multi-Threading:** Utilizes OpenMP to render scanlines concurrently, drastically reducing frame render times.
- **Materials:**
  - `lambertian` (Matte/Diffuse surfaces)
  - `metal` (Reflective surfaces with adjustable fuzziness)
  - `dielectric` (Refractive surfaces like glass and water)
  - `light_source` (Emissive materials)
- **Shapes:** Highly optimized sphere intersections.
- **Camera:** Fully configurable camera including field of view (FOV), aspect ratio, position (`lookfrom`, `lookat`), and multi-sampled anti-aliasing (MSAA).
- **Animation Engine:** Automatically loops through mathematical functions to update the scene, renders sequential `.ppm` frames, and hooks into `FFmpeg` to stitch them into a smooth 30fps `.mp4` video.

https://github.com/user-attachments/assets/371547e9-83cd-4eba-8644-dafdabecda60

## Project Structure

- `Headers/` - Contains the core mathematical and logic structures of the ray tracer.
  - `vec3.h` / `color.h` / `ray.h` - Core 3D math and vector operations.
  - `camera.h` - Camera logic, ray generation, and multi-threaded rendering loop.
  - `hittable.h` / `sphere.h` / `scene.h` - Object geometry and scene management.
  - `material.h` - Material scattering and light emission behavior.
- `main.cpp` - Renders a static showcase scene with glass, metal, and matte spheres to a single `output.ppm` image.
- `renderer.cpp` - Generates a rotating animated sequence of 300 frames and invokes FFmpeg to encode them into `out.mp4`.

## Prerequisites

Before building and running the project, make sure you have the following installed:

1. **C++17 Compiler:** GCC, Clang, or MSVC with OpenMP support.
2. **OpenMP:** Usually bundled with GCC/Clang (requires the `-fopenmp` flag).
3. **FFmpeg:** Must be installed and added to your system's `PATH` environment variable if you intend to run the animation renderer (`renderer.cpp`).

## Building & Running

### 1. Rendering a Static Image (`main.cpp`)
To compile the basic scene and generate a single `.ppm` file:

```bash
# Compile with high optimization (-O3), OpenMP, and the Headers directory
g++ -O3 -fopenmp -I Headers main.cpp -o main

# Run the executable
./main
```
*This will generate an `output.ppm` file in your directory.*

### 2. Rendering an Animation (`renderer.cpp`)
To compile the animated scene and generate a video:

```bash
# Compile with high optimization (-O3), OpenMP, and the Headers directory
g++ -O3 -fopenmp -I Headers renderer.cpp -o renderer

# Run the executable
./renderer
```
*This will:*
1. *Render 300 individual frame images (`frame000.ppm` to `frame299.ppm`).*
2. *Execute an FFmpeg system command to encode them into `out.mp4`.*
3. *Leave the frames and the video file in your working directory.*

## Notes on Performance

Ray tracing is a highly computationally expensive process. The `camera.h` relies on `samples_per_pixel` and `max_depth` to determine the quality of the image. 

If rendering the video takes too long, you can tweak the following inside `renderer.cpp`:
*   `cam.samples_per_pixel`: Lower this (e.g., to 10 or 50) for faster, but noisier renders.
*   `cam.max_depth`: Lower this (e.g., to 10) to reduce the number of light bounces.
*   `cam.image_width`: Lower the resolution (e.g., to 200).

Conversely, for a final, production-quality render, increase these values and grab a coffee ☕.
