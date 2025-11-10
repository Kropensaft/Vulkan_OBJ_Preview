#include "VulkanApplication.h"
#include "Window.h"
#include <stdexcept>

#if defined(__APPLE__)

#include "platforms/darwin/menu_darwin.h"
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3native.h>
#include <iostream>

#endif

void loadModelFromFile(const char *filepath) {
  std::cout << "Menu callback received. Filepath: " << filepath << std::endl;
  try {
    FileParser::parse_OBJ(filepath);
    std::cout << "Successfully loaded model: " << filepath << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "Failed to load model: " << e.what() << std::endl;
  }
}

GLFWwindow *Window::getGLFWWindow() {
  return instance->window;
}

void renderWireframe(void) {
  VulkanApplication::renderWireframe = !VulkanApplication::renderWireframe;
}

void Window::zoomIn(void) {
  if (instance && instance->window) {
    Camera *camera = static_cast<Camera *>(glfwGetWindowUserPointer(instance->window));
    if (camera) {
      float dt = VulkanApplication::getDeltaTime();
      camera->zoomIn(dt);
    }
  }
}
void Window::zoomOut(void) {
  if (instance && instance->window) {
    Camera *camera = static_cast<Camera *>(glfwGetWindowUserPointer(instance->window));
    if (camera) {
      float dt = VulkanApplication::getDeltaTime();
      camera->zoomOut(dt);
    }
  }
}
Window *Window::instance = nullptr;

Window::Window(int w, int h, std::string name) : width(w), height(h), windowName(name) {
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

  window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);

// INFO: Create menu bar using the native API's
#if defined(__APPLE__)
  void *native_window_handle = glfwGetCocoaWindow(window);
  create_macos_menu_bar(native_window_handle, &loadModelFromFile, &renderWireframe, &Window::zoomIn, &Window::zoomOut);
#endif
}

void Window::setupInputCallbacks() {
  glfwSetCursorPosCallback(window, mousePositionCallback);
  glfwSetMouseButtonCallback(window, mouseButtonCallback);
}

void Window::mousePositionCallback(GLFWwindow *window, double xpos, double ypos) {
  if (instance) {
    instance->mousePos.x = xpos;
    instance->mousePos.y = ypos;
  }
}

void Window::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
  if (instance && button >= 0 && button < GLFW_MOUSE_BUTTON_LAST) {
    instance->mouseButtons[button] = (action == GLFW_PRESS);
  }
}

void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {
  if ((glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)) {
    throw std::runtime_error("Failed to create window surface!");
  }
}
