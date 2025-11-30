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
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_SOURCE_DIR}/compile-shaders.sh
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Compiling GLSL shaders to SPIR-V"
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


if(APPLE)
    # This property enables the creation of a .app bundle.
    set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE TRUE)

    # Add the Objective-C++ source file needed for the macOS menu bar.
    target_sources(${PROJECT_NAME} PRIVATE src/platforms/darwin/menu_darwin.mm)

    # --- CRITICAL BUNDLE PROPERTIES ---
    # This section links the necessary metadata and permission files to the app.
    # Without these, the app cannot interact correctly with macOS services like
    # the file picker or have a proper identity.
    set_target_properties(${PROJECT_NAME} PROPERTIES
        # Link your custom Info.plist file to the bundle. This gives your app
        # its identity (e.g., com.example.vulkanapp).
        # Assumes Info.plist is in the root directory of your project.
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist"

        # Link the entitlements file. This is required for sandboxed apps to
        # get permission to do things like opening a file dialog.
        # Assumes VulkanApp.entitlements is in the root directory.
        XCODE_ATTRIBUTE_CODE_SIGN_ENTITLEMENTS "${CMAKE_CURRENT_SOURCE_DIR}/VulkanApp.entitlements"
    )

    # --- App Icon Handling ---
    set(APP_ICON_NAME "favicon.icns")
    set(APP_ICON_PATH "${CMAKE_SOURCE_DIR}/icon/${APP_ICON_NAME}")
    if(EXISTS ${APP_ICON_PATH})
        message(STATUS "Found app icon, adding to bundle: ${APP_ICON_PATH}")
        set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_ICON_FILE ${APP_ICON_NAME})
        # This ensures the icon file is correctly copied into the bundle's Resources folder.
        set_source_files_properties(${APP_ICON_PATH} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
        target_sources(${PROJECT_NAME} PRIVATE ${APP_ICON_PATH})
    else()
        message(WARNING "App icon not found at: ${APP_ICON_PATH}")
    endif()

    # --- System Framework Linking ---
    # Find and link the essential Cocoa and Foundation frameworks for macOS GUI apps.
    find_library(COCOA_FRAMEWORK Cocoa REQUIRED)
    find_library(FOUNDATION_FRAMEWORK Foundation REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${COCOA_FRAMEWORK} ${FOUNDATION_FRAMEWORK})

    # --- Post-Build Steps ---
    # These commands are run automatically after the main executable is built.
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_SOURCE_DIR}/compile-shaders.sh
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Compiling GLSL shaders to SPIR-V"
    )
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${PROJECT_SOURCE_DIR}/shaders"
            # This special path correctly finds the Resources folder inside the .app bundle.
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/../Resources/shaders"
        COMMENT "Copying shaders into app bundle"
    )
endif()
