#ifndef MENU_WIN
#define MENU_WIN

#include <functional>
#include <string>

using OpenFileCallback = std::function<void(const char *)>;
using RenderWireframeCallback = std::function<void()>;
using ZoomInCallback = std::function<void()>;
using ZoomOutCallback = std::function<void()>;
using SwitchLightSourceCallback = std::function<void()>;
using SetZoomSensitivityCallback = std::function<void(double sensitivity)>;

void create_macos_menu_bar(void *native_window_handle,
                           OpenFileCallback file_callback,
                           RenderWireframeCallback wireframeCallback,
                           ZoomInCallback zoomInCallback,
                           ZoomOutCallback zoomOutCallback,
                           SwitchLightSourceCallback switchLSCallback,
                           SetZoomSensitivityCallback zoomSensCallback);

#endif //! MENU_WIN
