# Retro3D

2.5D rendering project.

![screenshot](https://pbs.twimg.com/media/DAh1QhPXkAAH2Ea.jpg:large)


![screenshot](https://pbs.twimg.com/media/DAnZkWXXgAAHQsH.jpg:large)

Features:
- Raytraced 3D rendering, similar to old games like Wolfenstein (raytracing on a 2D grid).
- Level scripts and script components, using ChaiScript.
- WebAssembly support (can run in browser).

This is a hobby project, made for educational purposes.

# How to build

WINDOWS
- Open CMake GUI (can download from here: https://cmake.org/download/)
- In "Where is the srouce code", browse to the repository folder containing CMakeLists.txt
- In "Where to build the binaries", chose any folder (i.e. subfolder "/build")
- Make sure the directory can be written to (to prevent MSB3073 error)
- Configure
- Build
- Open Visual Studio project
- Build Retro3D project (IN RELEASE CONFIGURATION - Debug will be very slow!)

WEB ASSEMBLY
- Make sure you have Emscripten installed (https://developer.mozilla.org/en-US/docs/WebAssembly/C_to_wasm)
- Also make sure you have newest version of Python 2.x installed
- Run emcc_generate.py
- Run emcc_build.bat
- Find your webpage in the "webbuild" sub folder
