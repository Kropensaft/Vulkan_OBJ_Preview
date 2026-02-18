#if defined(_WIN32)

#include "menu_win.hpp"
#include <commdlg.h>
#include <windows.h>

// Define IDs for menu actions
#define IDM_FILE_OPEN 1001
#define IDM_VIEW_WIREFRAME 1002
#define IDM_ZOOM_IN 1003
#define IDM_ZOOM_OUT 1004
#define IDM_LS_SWITCH 1005

// Global callbacks to be triggered by the WndProc
static OpenFileCallback g_fileCb;
static RenderWireframeCallback g_wireframeCb;
static ZoomInCallback g_zoomInCb;
static ZoomOutCallback g_zoomOutCb;
static SwitchLightSourceCallback g_lsCb;
static WNDPROC g_OriginalWndProc = nullptr;

// Function to handle the Windows native file picker
void WindowsOpenFilePicker() {
  char szFile[260] = {0};
  OPENFILENAMEA ofn;
  ZeroMemory(&ofn, sizeof(ofn));
  ofn.lStructSize = sizeof(ofn);
  ofn.lpstrFile = szFile;
  ofn.nMaxFile = sizeof(szFile);
  ofn.lpstrFilter = "Object Files\0*.obj\0All Files\0*.*\0";
  ofn.nFilterIndex = 1;
  ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

  if (GetOpenFileNameA(&ofn)) {
    if (g_fileCb)
      g_fileCb(ofn.lpstrFile);
  }
}

// Intercepts messages before GLFW handles them
LRESULT CALLBACK MenuWndProc(HWND hwnd, UINT msg, WPARAM wParam,
                             LPARAM lParam) {
  if (msg == WM_COMMAND) {
    switch (LOWORD(wParam)) {
    case IDM_FILE_OPEN:
      WindowsOpenFilePicker();
      return 0;
    case IDM_VIEW_WIREFRAME:
      if (g_wireframeCb)
        g_wireframeCb();
      return 0;
    case IDM_ZOOM_IN:
      if (g_zoomInCb)
        g_zoomInCb();
      return 0;
    case IDM_ZOOM_OUT:
      if (g_zoomOutCb)
        g_zoomOutCb();
      return 0;
    case IDM_LS_SWITCH:
      if (g_lsCb)
        g_lsCb();
      return 0;
    }
  }
  return CallWindowProc(g_OriginalWndProc, hwnd, msg, wParam, lParam);
}

void create_windows_menu_bar(void *native_window_handle,
                             OpenFileCallback file_callback,
                             RenderWireframeCallback wireframeCallback,
                             ZoomInCallback zoomInCallback,
                             ZoomOutCallback zoomOutCallback,
                             SwitchLightSourceCallback switchLSCallback,
                             SetZoomSensitivityCallback zoomSensCallback) {

  HWND hwnd = static_cast<HWND>(native_window_handle);
  if (!hwnd)
    return;

  // Store callbacks globally for the WndProc
  g_fileCb = file_callback;
  g_wireframeCb = wireframeCallback;
  g_zoomInCb = zoomInCallback;
  g_zoomOutCb = zoomOutCallback;
  g_lsCb = switchLSCallback;

  // Create Menus
  HMENU hMenu = CreateMenu();
  HMENU hFileMenu = CreatePopupMenu();
  AppendMenuA(hFileMenu, MF_STRING, IDM_FILE_OPEN, "&Open .obj File...");
  AppendMenuA(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, "&File");

  HMENU hViewMenu = CreatePopupMenu();
  AppendMenuA(hViewMenu, MF_STRING, IDM_VIEW_WIREFRAME, "&Toggle Wireframe");
  AppendMenuA(hViewMenu, MF_STRING, IDM_ZOOM_IN, "Zoom &In");
  AppendMenuA(hViewMenu, MF_STRING, IDM_ZOOM_OUT, "Zoom &Out");
  AppendMenuA(hViewMenu, MF_STRING, IDM_LS_SWITCH, "&Switch Light Source");
  AppendMenuA(hMenu, MF_POPUP, (UINT_PTR)hViewMenu, "&View");

  SetMenu(hwnd, hMenu);

  // Subclass the window
  g_OriginalWndProc =
      (WNDPROC)SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)MenuWndProc);
}

#endif
