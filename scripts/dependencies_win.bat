@echo off
echo Installing dependencies for Vulkan Tutorial...

:: 1. Check for CMake
where cmake >nul 2>nul
if %ERRORLEVEL% neq 0 (
    echo [ERROR] CMake not found. Please install CMake from https://cmake.org/download/
    exit /b 1
)
echo Found CMake.

:: 2. Check if vcpkg is in PATH or current directory
where vcpkg >nul 2>nul
if %ERRORLEVEL% equ 0 (
    set VCPKG_EXE=vcpkg
) else if exist ".\vcpkg.exe" (
    set VCPKG_EXE=.\vcpkg
) else (
    echo [ERROR] vcpkg not found.
    echo Please git clone https://github.com/Microsoft/vcpkg.git and run .\bootstrap-vcpkg.bat
    echo After installing you have to add the folder path to 'System Environment variables' under the key PATH
    echo This can be done via the Windows GUI when searching for System Environment
    exit /b 1
)

:: 3. Enable binary caching
echo Enabling binary caching for vcpkg...
set VCPKG_BINARY_SOURCES=clear;files,%TEMP%\vcpkg-cache,readwrite
if not exist %TEMP%\vcpkg-cache mkdir %TEMP%\vcpkg-cache

:: 4. Install dependencies
echo Installing libraries via vcpkg...
%VCPKG_EXE% install glfw3 glm tinyobjloader stb tinygltf nlohmann-json ktx[vulkan] openal-soft --triplet=x64-windows

:: 5. Final Instructions
echo.
echo Don't forget to install the Vulkan SDK from https://vulkan.lunarg.com/
echo.
echo All dependencies installed!
echo To build, replace [path\to\vcpkg] with the path where you installed it and run this:
echo cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=[path\to\vcpkg]\scripts\buildsystems\vcpkg.cmake
echo after this successfully completes run: 
echo cmake --build build
echo the application will be inside .\build\VulkanApplication.exe
exit /b 0
