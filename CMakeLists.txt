# Minimum required version of CMake
cmake_minimum_required(VERSION 3.12)

# Project name and language
project(VulkanApp CXX)

# --- ADD THIS LINE ---
# Always generate the compile_commands.json file for the language server (clangd)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set C++ standard to 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find necessary packages
find_package(Vulkan REQUIRED)
find_package(glfw3 3.3 REQUIRED)

# List all source files from the src directory
set(SOURCES
    src/main.cpp
    src/Window.cpp
    src/VulkanApplication.cpp
    src/FileParser.cpp
  )

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

# Handle Apple-specific frameworks for the target
if(APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE "-framework Cocoa" "-framework IOKit" "-framework Foundation")
endif()
