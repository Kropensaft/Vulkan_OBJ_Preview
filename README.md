# Vulkan OBJ Previewer

![C++](https://img.shields.io/badge/Language-C++20-blue.svg)
![Vulkan](https://img.shields.io/badge/API-Vulkan-red.svg)
![Platform](https://img.shields.io/badge/Platform-macOS%20|%20Windows-lightgrey.svg)
![License](https://img.shields.io/badge/License-MIT-green.svg)

A high-performance, cross-platform 3D model viewer built from scratch using the Vulkan API and C++. This application allows users to load, inspect, and interact with .obj 3D models and their associated .mtl materials in real-time.

## Key Features

* Custom OBJ & MTL Parser: A fast, hand-written parser that loads geometry, normals, texture coordinates, and material properties without relying on heavy external asset-loading libraries.
* Vulkan Rendering Pipeline: Fully utilizes the Vulkan API for low-overhead, high-efficiency rendering.
* Dynamic Shadow Mapping: Features a dedicated shadow render pass and pipeline for realistic directional lighting and depth.
* Native OS Menus: Seamlessly integrates with the host operating system using native menu bars.
  * macOS: Objective-C++ (Cocoa / AppKit) integration.
  * Windows: Win32 API and modern COM-based IFileOpenDialog.
* Interactive Camera: Intuitive 3D camera controls with customizable zoom sensitivity.
* Wireframe Mode: Toggleable polygon rendering modes (Fill vs. Line) for geometry inspection.
* Extensive Documentation: Fully documented codebase using Doxygen (with interactive UML and Call graphs).

## Screenshots

## Prerequisites

Before building the project, you must manually install the following core tools:

* C++ Compiler: A compiler supporting C++20 (e.g., MSVC for Windows, Apple Clang for macOS).
* CMake: Version 3.15 or newer.
* Vulkan SDK: Must be installed on your system. Ensure the `VULKAN_SDK` environment variable is correctly set after installation.

### Platform-Specific Prerequisites

To allow the dependency scripts to fetch required C++ libraries (like GLFW and GLM), you need the following package managers:

* Windows: You must install `vcpkg`. Make sure it is properly set up and integrated with your environment (e.g., by running `vcpkg integrate install`).
* macOS: You must have `Homebrew` installed, as the macOS dependency script relies on it.

## Building the Project

### 1. Fetch External Dependencies
Once you have the manual prerequisites installed, run the respective dependency script for your operating system. These scripts will automatically fetch and configure external libraries (such as GLFW and GLM).

macOS:
```bash
chmod +x scripts/dependencies_darwin.sh
./scripts/dependencies_darwin.sh
```

Windows:
```cmd
scripts\dependencies_win.bat
```

### 2. Compile Shaders
Vulkan consumes shaders in the SPIR-V bytecode format. Compile the GLSL shaders using the provided script:
```bash
chmod +x compile-shaders.sh
./compile-shaders.sh
```

### 3. Build with CMake
```bash
mkdir build && cd build
cmake ..
cmake --build .
```

## Usage & Controls

Once built, run the executable. By default, the application will load a fallback model (e.g., teapot.obj).

### Interaction
* Rotate Camera: Click and drag the Left Mouse Button.
* Zoom In / Out: Scroll the Mouse Wheel.

### Native Menu Bar Actions
Use the native application menu (at the top of the screen on macOS, or under the window title bar on Windows) to:
* File -> Open .obj File...: Load a new 3D model from your disk.
* View -> Toggle Wireframe: Switch between solid rendering and wireframe mesh.
* View -> Zoom In / Zoom Out: Alternative zooming method.
* View -> Switch Light Source: Re-orient the directional light to match the current camera view direction.
* View -> Zoom Sensitivity: Adjust the scroll wheel zoom speed via a native slider.

## Documentation

This project uses Doxygen to generate a comprehensive HTML documentation of the internal architecture.

To generate the documentation locally:
1. Ensure `doxygen` and `graphviz` are installed on your machine.
2. Run the following command in the root directory:
   ```bash
   doxygen Doxyfile
   ```
3. Open `html/index.html` in your preferred web browser.

## License
This project is open-source and available under the MIT License.
