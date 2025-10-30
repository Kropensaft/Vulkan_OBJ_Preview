#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

class Window {
public:
  Window(int w, int h, std::string name);
  ~Window();

  Window(const Window &) = delete;
  Window &operator=(const Window &) = delete;

  bool shouldClose() const { return glfwWindowShouldClose(window); }
  void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
  VkExtent2D getExtent() const { return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; }

private:
  void initWindow();

  const int width;
  const int height;
  std::string windowName;
  GLFWwindow *window;
};
