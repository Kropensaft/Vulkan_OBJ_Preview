#include "InputController.h"

// INFO: GLFW_CURSOR_NORMAL doesnt hide the cursor and does not restrain it to
// the window
InputController::InputController(Window &window, Camera &camera)
    : m_window(window), m_camera(camera) {
  glfwSetInputMode(m_window.getGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void InputController::processInput(float deltaTime) {
  GLFWwindow *glfwWindow = m_window.getGLFWWindow();

  // INFO: Exit via ESC key
  if (glfwGetKey(glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(glfwWindow, true);
  }

  // INFO: User has to press the button and hold (like in blender)
  if (glfwGetMouseButton(glfwWindow, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
    static double xpos, ypos;
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
  } else {

    // INFO: stops mouse jumping
    m_firstMouse = true;
  }
}
