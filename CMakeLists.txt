# Minimum required version of CMake
cmake_minimum_required(VERSION 3.12)

# Project name and language
project(VulkanApp CXX)

# --- ADD THIS LINE ---
# Always generate the compile_commands.json file for the language server (clangd)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set C++ standard to 17
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find necessary packages
find_package(Vulkan REQUIRED)
find_package(glfw3 3.3 REQUIRED)

# List all source files from the src directory
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Create the executable target
add_executable(${PROJECT_NAME} ${SOURCES})

add_custom_command(
  TARGET VulkanApp
  POST_BUILD
  COMMENT "################\nCompiling shaders"
  COMMAND ".././compile-shaders.sh" 
)

# Tell the target where its header files are
target_include_directories(${PROJECT_NAME} PUBLIC
    "${PROJECT_SOURCE_DIR}/include"
)

# Link all the necessary libraries to the target
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Vulkan::Vulkan
        glfw
)

# NOTE: Apple specific build process
if (APPLE)
    # Add the Objective-C++ source file to your executable
    target_sources(${PROJECT_NAME} PRIVATE src/platforms/darwin/menu_darwin.mm)

    # Find and link the Cocoa framework, which provides all the NSMenu, NSWindow, etc. APIs
    find_library(COCOA_FRAMEWORK Cocoa REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${COCOA_FRAMEWORK})

    # You might also need to add this for Objective-C runtime features
    find_library(FOUNDATION_FRAMEWORK Foundation REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${FOUNDATION_FRAMEWORK})
endif()
