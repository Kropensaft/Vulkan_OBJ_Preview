#ifndef WINDOW_H
#define WINDOW_H
#include "FileParser.h"
#include <glm/glm.hpp>
#include <string>
#include <vulkan/vulkan_core.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
class Window {
public:
  Window(int w, int h, std::string name);
  ~Window();

  Window(const Window &) = delete;
  Window &operator=(const Window &) = delete;

  static GLFWwindow *getGLFWWindow();
  bool shouldClose() const {
    return glfwWindowShouldClose(window);
  }
  void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

  VkExtent2D getExtent() const {
    return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
  }
  glm::dvec2 getMousePosition() const {
    return mousePos;
  }

  bool isMouseButtonPressed(int button) const {
    return (button >= 0 && button < GLFW_MOUSE_BUTTON_LAST) ? mouseButtons[button] : false;
  }

private:
  void initWindow();
  void setupInputCallbacks();
  glm::dvec2 mousePos = {0.0, 0.0};
  bool mouseButtons[GLFW_MOUSE_BUTTON_LAST] = {false};

  const int width;
  const int height;
  std::string windowName;

  static void zoomIn(void);
  static void zoomOut(void);

  static void mousePositionCallback(GLFWwindow *window, double xpos, double ypos);
  static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
  GLFWwindow *window;
  static Window *instance;
};

#endif
