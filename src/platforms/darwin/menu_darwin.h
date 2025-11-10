#ifndef MENU_DARWIN_H
#define MENU_DARWIN_H
// Use extern "C" to ensure C-style linkage, making it callable from the .mm file's C implementation
#ifdef __cplusplus
extern "C" {
#endif

// A function pointer type for the callback.
// The C++ code will provide a function of this type.
typedef void (*OpenFileCallback)(const char *filepath);

typedef void (*RenderWireframeCallback)(void);

typedef void (*ZoomInCallback)(void);

typedef void (*ZoomOutCallback)(void);
/**
 * @brief Creates the native macOS menu bar.
 *
 * @param native_window_handle A pointer to the native NSWindow.
 * @param callback The function to be called when the "Open" menu item is clicked.
 */
void create_macos_menu_bar(void *native_window_handle,
                           OpenFileCallback file_callback,
                           RenderWireframeCallback wireframeCallback,
                           ZoomInCallback zoomInCallback,
                           ZoomOutCallback zoomOutCallback);

#ifdef __cplusplus
}
#endif

#endif // !MENU_DARWIN_H
