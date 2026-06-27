# RayTracer

A C++ CPU ray tracer that renders text-defined 3D scenes to PNG images. The project demonstrates core computer graphics work: scene parsing, camera setup, geometric transforms, ray-object intersection, direct lighting, shadows, reflections, and image export.

This repository is most useful as a graphics/engine programming sample. It shows implementation-level understanding of ray tracing rather than relying on a rendering framework to do the hard parts.

## What It Does

- Parses scene description files with camera, image size, output file, lighting, material, transform, sphere, vertex, and triangle commands.
- Supports spheres and triangle meshes through a shared `Shape` interface.
- Applies object transforms with a matrix stack using `pushTransform` and `popTransform`.
- Computes perspective camera rays from the parsed camera settings.
- Calculates diffuse and specular lighting with point and directional lights.
- Casts shadow rays for visibility checks.
- Supports recursive mirror-style reflections up to the configured internal max depth.
- Handles attenuation for point lights.
- Uses inverse-transpose normal transformation for transformed geometry.
- Parallelizes the pixel loop with OpenMP.
- Writes rendered output as PNG files through FreeImage.

## Technical Highlights

- **Language:** C++17
- **Math:** GLM vectors and matrices
- **Rendering:** CPU ray tracing
- **Primitives:** Analytic spheres and indexed triangles
- **Lighting:** Ambient, diffuse, specular, emission, point lights, directional lights, attenuation, shadows
- **Transforms:** Translation, rotation, scale, matrix stack, transformed intersections
- **Output:** PNG export via FreeImage
- **Parallelism:** OpenMP over image pixels

## Repository Layout

```text
.
├── main.cpp              # Console entry point, scene loading prompt, PNG export
├── display.cpp           # Ray generation, tracing, lighting, shadows, reflections
├── readfile.cpp/.h       # Scene file parser and scene initialization
├── Shape.h               # Base interface for renderable geometry
├── Sphere.h/.cpp         # Sphere primitive and ray intersection
├── Triangle.h/.cpp       # Triangle primitive and ray intersection
├── Transform.cpp/.h      # Camera and transformation matrix helpers
├── variables.h           # Shared scene state
├── scenes/               # Sample scene files
├── include/              # Bundled OpenGL/FreeImage headers
├── lib/                  # Bundled Windows graphics libraries/DLLs
└── CMakeLists.txt        # CMake project definition
```

## Scene Format

Scene files are plain text. Blank lines and lines beginning with `#` are ignored.

Common commands include:

```text
size <width> <height>
camera <eyeX> <eyeY> <eyeZ> <centerX> <centerY> <centerZ> <upX> <upY> <upZ> <fovy>
output <filename.png>

ambient <r> <g> <b>
diffuse <r> <g> <b>
specular <r> <g> <b>
emission <r> <g> <b>
shininess <value>
attenuation <constant> <linear> <quadratic>

point <x> <y> <z> <r> <g> <b>
directional <x> <y> <z> <r> <g> <b>

vertex <x> <y> <z>
tri <v0> <v1> <v2>
sphere <x> <y> <z> <radius>

pushTransform
translate <x> <y> <z>
rotate <x> <y> <z> <degrees>
scale <x> <y> <z>
popTransform
```

Example:

```text
size 320 240
camera 0 0 1 0 0 -1 0 1 0 45
output scene6.png

point 0 0.44 -1.5 0.8 0.8 0.8
attenuation 1 0.1 0.05

diffuse 0.7 0.5 0.2
specular 0.2 0.2 0.2
shininess 20

pushTransform
translate 0 -0.7 -1.5
scale 0.1 0.1 0.1
sphere 0 0 0 1
popTransform
```

Sample scenes are available in `scenes/`, including material tests, a Cornell-box-style scene, a thousand-sphere stress scene, and a Stanford Dragon triangle scene.

## Build Notes

The project is set up as a CMake C++17 executable, but the current `CMakeLists.txt` contains absolute Windows library paths such as:

```text
C:/Users/Distjoy/CLionProjects/RayTracer/lib/FreeImage.dll
C:/Users/Distjoy/CLionProjects/RayTracer/lib/freeglut.dll
```

That means the library paths may need to be adjusted before building on a different machine. The repository already includes headers and several Windows library/DLL files under `include/`, `lib/`, and `glut/`.

Expected dependencies:

- CMake 3.27 or newer
- C++17 compiler
- OpenGL
- OpenMP-capable compiler/runtime
- GLM
- FreeImage
- GLEW/freeglut or compatible OpenGL headers/libraries

Typical CMake flow after fixing local library paths:

```bash
cmake -S . -B build
cmake --build build
```

## Running

Run the executable from a terminal. The program displays a small keyboard menu:

```text
press 'h' to print this message again.
press 'r' to start ray tracing
press ESC to quit.
```

Press `r`, then enter the path to a `.test` scene file when prompted. The key handler exits on `q`; the prompt text currently says ESC.

The renderer writes the PNG named by the scene's `output` command. With Windows-style full paths, the output is written alongside the scene file. With relative or Unix-style paths, it is written relative to the current working directory.

Example scene paths:

```text
scenes/scene4-ambient.test
scenes/scene4-diffuse.test
scenes/scene4-specular.test
scenes/scene5.test
scenes/scene6.test
scenes/scene7.test
```

## Code Guide

This project is a compact educational renderer with most of the interesting rendering logic written directly in C++. The strongest areas to inspect are:

- `display.cpp` for ray generation, recursive tracing, lighting, shadows, and OpenMP usage.
- `Sphere.h` and `Triangle.cpp` for intersection implementations.
- `readfile.cpp` for the scene parser and transform stack.
- `Transform.cpp` for camera, rotation, scaling, and translation math.

The codebase also shows areas that could be improved in production work: replacing global scene state, making the build portable, improving memory ownership for render buffers, adding automated image tests, and separating parsing from rendering state.
