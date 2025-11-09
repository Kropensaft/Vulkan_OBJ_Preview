#include "InputController.h"

InputController::InputController(Window &window, Camera &camera)
    : m_window(window), m_camera(camera) {
  glfwSetInputMode(m_window.getGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

void InputController::processInput(float deltaTime) {
  GLFWwindow *glfwWindow = m_window.getGLFWwindow();

  if (glfwGetKey(glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(glfwWindow, true);
  }

  double xpos, ypos;
  glfwGetCursorPos(glfwWindow, &xpos, &ypos);

  if (m_firstMouse) {
    m_lastX = xpos;
    m_lastY = ypos;
    m_firstMouse = false;
  }

  float xoffset = xpos - m_lastX;
  float yoffset = m_lastY - ypos;

  m_lastX = xpos;
  m_lastY = ypos;

  m_camera.UpdateCamera(glfwWindow, xoffset, yoffset, deltaTime);
}
