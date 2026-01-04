# Stratum - Point Cloud Engine

Stratum is a C++ rendering engine designed to visualize massive point cloud datasets in real-time. I built this over the winter'26 break to dive deeper into low-level graphics programming, memory management, and the OpenGL pipeline.
 
The goal was simple: take raw coordinate data (x, y, z) and render it at 60 FPS without crashing the system, even with millions of points.

https://github.com/user-attachments/assets/cbf77ef9-b1e9-4c90-8dd2-ae486ffd4480


## Key Features
* **Real-time Rendering:** Handles 10,000,000+ points smoothly using Modern OpenGL (Core Profile).
* **Level of Detail (LOD):** Implemented a custom grid-based spatial partitioning system. It culls chunks that are too far away and reduces point density for distant objects to save performance.
* **Async Asset Loading:** Uses `std::future` and worker threads to load data in the background, so the window opens instantly and doesn't freeze while parsing files.
* **Interactive UI:** Integrated **Dear ImGui** to control shader parameters like height map colors and point size on the fly.
* **Custom Shaders:** Wrote GLSL vertex and fragment shaders to create a dynamic height-based heatmap (Blue -> Green -> White).

## Technical Implementation

### 1. Multithreading
One of the big issues I ran into early on was that parsing a large text file blocks the main thread. I refactored the `DataLoader` to use smart pointers (`std::unique_ptr`) and `std::async`. The data is loaded in a seperate thread, and ownership is transferred to the main renderer only when its ready. This keeps the application responsive during the heavy I/O operations.

### 2. Optimization (LOD)
To handle the scale, I split the terrain into a 4x4 grid of "Chunks".
Instead of swapping meshes, I used a statistical trick: I shuffle the points inside the buffers. This allows me to simply draw less points (e.g. the first 10% of the buffer) when the camera is far away, and it looks like a uniform reduction in density rather than just cutting off the geometry.

## Controls
* **Left Click + Drag:** Rotate Camera (Orbit)
* **Scroll:** Zoom In/Out
* **ImGui Panel:**
    * Adjust `Sea Level` (Min Height) and `Snow Level` (Max Height) to change the gradient.
    * Change background color and point size.

## Build Instructions
This project uses CMake. You need a C++17 compliant compiler.

```bash
mkdir build
cd build
cmake ..
cmake --build .
```
