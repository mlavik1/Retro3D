<p align="center"><img src="resources/logo/logotype.png" alt="Retro3D" height="160px"></p>

# 2.5D (pseudo-3D) game engine

2.5D retro game engine. Old school software 3D rendering.

Builds and runs on Windows, Linux and WebAssembly.

By Matias Lavik

TRY IT HERE: http://dungeoncrawler.dx.am/ (Requires a browser with webassembly-support)

![screenshot](https://pbs.twimg.com/media/DAh1QhPXkAAH2Ea.jpg:large)


![screenshot](https://pbs.twimg.com/media/DAnZkWXXgAAHQsH.jpg:large)

Features:
- Raytraced 3D rendering, similar to old games like Wolfenstein (raytracing on a 2D grid).
- Level scripts and script components, using ChaiScript.
- WebAssembly support (can run in browser).

# How to build

__WINDOWS__
- Open CMake GUI (can download from here: https://cmake.org/download/)
- In "Where is the srouce code", browse to the repository folder containing CMakeLists.txt
- In "Where to build the binaries", chose any folder (i.e. subfolder "/build")
- Make sure the directory can be written to (to prevent MSB3073 error)
- Configure
- Build
- Open Visual Studio project
- Build and run the "Game" project (IN RELEASE CONFIGURATION - Debug will be very slow!)

__LINUX__
- Run aptget.sh (or manually do apt-get on the dependencies)
- Run CMake
- Build the "Game" project (or make your own project based on that)

__WEB ASSEMBLY__
- Make sure you have Emscripten installed (https://developer.mozilla.org/en-US/docs/WebAssembly/C_to_wasm)
- Also make sure you have newest version of Python 2.x installed
- Run emcc_generate.py
- Run emcc_build.bat
- Find your webpage in the "webbuild" sub folder

# Contributing

Both code contributions, art contributions, bug reports and feature suggestions are more than welcome!
See [CONTRIBUTING.md](CONTRIBUTING.md) for more information.

Thanks to Sebastian Genlund for suggestions and useful feedback!
