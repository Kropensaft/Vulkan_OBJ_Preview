cmake_minimum_required(VERSION 3.12)

project(VulkanApp CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Vulkan REQUIRED)
find_package(glfw3 3.3 REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/include")

target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Vulkan glfw)


if(APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE TRUE)

    set(APP_ICON_NAME "favicon.icns")
    set(APP_ICON_PATH "${CMAKE_SOURCE_DIR}/icon/${APP_ICON_NAME}")

    if(EXISTS ${APP_ICON_PATH})
        message(STATUS "Found app icon, adding to bundle: ${APP_ICON_PATH}")
        set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_ICON_FILE ${APP_ICON_NAME})
        set_source_files_properties(${APP_ICON_PATH} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
        target_sources(${PROJECT_NAME} PRIVATE ${APP_ICON_PATH})
    else()
        message(WARNING "App icon not found at: ${APP_ICON_PATH}")
    endif()

    target_sources(${PROJECT_NAME} PRIVATE src/platforms/darwin/menu_darwin.mm)

    find_library(COCOA_FRAMEWORK Cocoa REQUIRED)
    find_library(FOUNDATION_FRAMEWORK Foundation REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${COCOA_FRAMEWORK} ${FOUNDATION_FRAMEWORK})

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
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/../Resources/shaders"
        COMMENT "Copying shaders into app bundle"
    )

endif()
