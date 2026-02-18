#ifndef MENU_DARWIN_H
#define MENU_DARWIN_H
#ifdef __cplusplus
extern "C" {
#endif

constexpr int ZOOM_SLIDER_MAX = 20;
constexpr double ZOOM_SLIDER_MIN = 0.1;

typedef void (*OpenFileCallback)(const char *filepath);

typedef void (*RenderWireframeCallback)(void);

typedef void (*ZoomInCallback)(void);

typedef void (*ZoomOutCallback)(void);

typedef void (*SwitchLightSourceCallback)(void);

typedef void (*SetZoomSensitivityCallback)(double sensitivity);
/**
 * @brief Creates the native macOS menu bar.
 *
 * @param native_window_handle A pointer to the native NSWindow.
 * @param callback The function to be called when the "Open" menu item is
 * clicked.
 */
void create_macos_menu_bar(void *native_window_handle,
                           OpenFileCallback file_callback,
                           RenderWireframeCallback wireframeCallback,
                           ZoomInCallback zoomInCallback,
                           ZoomOutCallback zoomOutCallback,
                           SwitchLightSourceCallback switchLSCallback,
                           SetZoomSensitivityCallback zoomSensCallback);

#ifdef __cplusplus
}
#endif

#endif // !MENU_DARWIN_H
