#include "Window.h"
#include <stdexcept>

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
