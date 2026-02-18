#include "Window.h"
#include "CameraController.h"
#include "VulkanApplication.h"
#include <stdexcept>

#if defined(__APPLE__)

#include "platforms/darwin/menu_darwin.h"
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3native.h>
#include <iostream>

#elif defined(__WIN32)
#include "platforms/windows/menu_win.hpp"
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#endif

void loadModelFromFile(const char *filepath) {
  std::cout << "Menu callback received. Filepath: " << filepath << std::endl;
  try {
    FileParser::parse_OBJ(filepath);
    std::cout << "Successfully loaded model: " << filepath << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "Failed to load model: " << e.what() << std::endl;
  }
  VulkanApplication::getInstance()->recreateGeometryBuffers();
}

void renderWireframe(void) { VulkanApplication::toggleWireframe(); }

void Window::zoomIn(void) { VulkanApplication::zoomIn(); }

void Window::zoomOut(void) { VulkanApplication::zoomOut(); }

void Window::switchLS(void) { VulkanApplication::switchLightSourcePosition(); }

void Window::setZoom(double sensitivity) {
  VulkanApplication::setZoomSpeed(sensitivity);
}

Window *Window::instance = nullptr;

Window::Window(int w, int h, std::string name)
    : width(w), height(h), windowName(name) {
  instance = this;
  initWindow();
  setupInputCallbacks();
}

Window::~Window() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

void Window::initWindow() {
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  window =
      glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);

#if defined(__APPLE__)
  void *native_window_handle = glfwGetCocoaWindow(window);
  create_macos_menu_bar(native_window_handle, &loadModelFromFile,
                        &renderWireframe, &Window::zoomIn, &Window::zoomOut,
                        &Window::switchLS, &Window::setZoom);
#elif defined(_WIN32)
  void *native_window_handle = (void *)glfwGetWin32Window(window);
  create_windows_menu_bar(native_window_handle, &loadModelFromFile,
                          &renderWireframe, &Window::zoomIn, &Window::zoomOut,
                          &Window::switchLS, &Window::setZoom);
#endif
}

GLFWwindow *Window::getGLFWWindow() { return instance->window; }

void Window::setupInputCallbacks() {
  glfwSetCursorPosCallback(window, mousePositionCallback);
  glfwSetMouseButtonCallback(window, mouseButtonCallback);
}

void Window::mousePositionCallback(GLFWwindow *window, double xpos,
                                   double ypos) {
  if (instance) {
    instance->mousePos.x = xpos;
    instance->mousePos.y = ypos;
  }
}

void Window::mouseButtonCallback(GLFWwindow *window, int button, int action,
                                 int mods) {
  if (instance && button >= 0 && button < GLFW_MOUSE_BUTTON_LAST) {
    instance->mouseButtons[button] = (action == GLFW_PRESS);
  }
}

void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {
  if ((glfwCreateWindowSurface(instance, window, nullptr, surface) !=
       VK_SUCCESS)) {
    throw std::runtime_error("Failed to create window surface!");
  }
}
