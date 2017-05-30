# Retro3D

2.5D rendering project.

![screenshot](https://pbs.twimg.com/media/DAh1QhPXkAAH2Ea.jpg:large)


![screenshot](https://pbs.twimg.com/media/DAnZkWXXgAAHQsH.jpg:large)

# How to build

WINDOWS
- Open CMake GUI (can download from here: https://cmake.org/download/)
- In "Where is the srouce code", browse to the repository folder containing CMakeLists.txt
- In "Where to build the binaries", chose any folder (i.e. subfolder "/build")
- Configure
- Build
- Open Visual Studio project
- Build
- Move the "resources" folder to the build folder (where the .exe-files were built)

WEB ASSEMBLY
- Make sure you have Emscripten installed (https://developer.mozilla.org/en-US/docs/WebAssembly/C_to_wasm)
- Also make sure you have newest version of Python 2.x installed
- Run emcc_generate.py
- Run emcc_build.bat
- Find your webpage in the "webbuild" sub folder
