/**
 * @file Window.h
 * @brief Wrapper class for GLFW window management.
 */
#ifndef WINDOW_H
#define WINDOW_H
#include "FileParser.h"
#include <glm/glm.hpp>
#include <string>
#include <vulkan/vulkan_core.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

/**
 * @class Window
 * @brief Class encapsulating GLFW window creation and input handling.
 * @details Provides an interface for initializing the window, polling states,
 * handling mouse/keyboard input, and creating a Vulkan surface.
 */
class Window {
public:
  /**
   * @brief Constructs a new application window.
   * @param w Width of the window in pixels.
   * @param h Height of the window in pixels.
   * @param name Title of the window.
   */
  Window(int w, int h, std::string name);

  /** @brief Destructor, handles window destruction and GLFW termination. */
  ~Window();

  Window(const Window &) = delete;
  Window &operator=(const Window &) = delete;

  /** @brief Returns a pointer to the underlying GLFW window. */
  static GLFWwindow *getGLFWWindow();

  /** @brief Checks if the window has been instructed to close. */
  bool shouldClose() const { return glfwWindowShouldClose(window); }

  /** @brief Creates a Vulkan surface for this window. */
  void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

  /** @brief Retrieves the current window extents (width and height). */
  VkExtent2D getExtent() const {
    return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
  }

  /** @brief Retrieves the current mouse position. */
  glm::dvec2 getMousePosition() const { return mousePos; }

  /** @brief Checks if a specific mouse button is currently pressed. */
  bool isMouseButtonPressed(int button) const {
    return (button >= 0 && button < GLFW_MOUSE_BUTTON_LAST)
               ? mouseButtons[button]
               : false;
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
  static void switchLS(void);
  static void setZoom(double);

  GLFWwindow *window;
  static Window *instance;
  static void mousePositionCallback(GLFWwindow *window, double xpos,
                                    double ypos);
  static void mouseButtonCallback(GLFWwindow *window, int button, int action,
                                  int mods);
};
#endif
