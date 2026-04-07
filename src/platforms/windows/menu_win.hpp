/**
 * @file menu_win.hpp
 * @brief Native Windows menu bar integration.
 */
#ifndef MENU_WIN
#define MENU_WIN

#include <functional>
#include <string>

// Callback type definitions for native menu actions
using OpenFileCallback = std::function<void(const char *)>;
using RenderWireframeCallback = std::function<void()>;
using ZoomInCallback = std::function<void()>;
using ZoomOutCallback = std::function<void()>;
using SwitchLightSourceCallback = std::function<void()>;
using SetZoomSensitivityCallback = std::function<void(double sensitivity)>;

/**
 * @brief Creates the native menu bar for the Windows application.
 * @param native_window_handle Pointer to the native window handle (HWND).
 * @param file_callback Callback triggered when 'Open' is selected.
 * @param wireframeCallback Callback to toggle wireframe rendering.
 * @param zoomInCallback Callback for zooming in.
 * @param zoomOutCallback Callback for zooming out.
 * @param switchLSCallback Callback to switch the light source position.
 * @param zoomSensCallback Callback to adjust zoom sensitivity.
 */
void create_macos_menu_bar(void *native_window_handle,
                           OpenFileCallback file_callback,
                           RenderWireframeCallback wireframeCallback,
                           ZoomInCallback zoomInCallback,
                           ZoomOutCallback zoomOutCallback,
                           SwitchLightSourceCallback switchLSCallback,
                           SetZoomSensitivityCallback zoomSensCallback);

#endif //! MENU_WIN
