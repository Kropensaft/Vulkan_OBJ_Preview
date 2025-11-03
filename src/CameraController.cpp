#include "CameraController.h"
#include "InputController.h"
#include "Storage.h"
#include <iostream>

Camera::Camera(glm::vec3 &&eye, glm::vec3 &&lookat, glm::vec3 &&upVector)
    : m_eye(std::move(eye)), m_lookAt(std::move(lookat)), m_upVector(std::move(upVector)) {
  UpdateViewMatrix();
}

void Camera::SetCameraView(glm::vec3 eye, glm::vec3 lookat, glm::vec3 up) {
  m_eye = eye;
  m_lookAt = lookat;
  m_upVector = up;
  UpdateViewMatrix();
}

void Camera::CameraInit() {
  glm::vec3 eye(5.0f, 5.0f, 5.0f);
  glm::vec3 lookat(0.0f, 0.0f, 0.0f); // Keep looking at the origin
  glm::vec3 up(0.0f, 1.0f, 0.0f);     // Keep Y as the up direction
  SetCameraView(eye, lookat, up);
  // Initialize mouse tracking variables
  lastMouseX = 0.0f;
  lastMouseY = 0.0f;
  firstMouse = true;
}

void Camera::UpdateViewMatrix() {
  m_viewMatrix = glm::lookAt(m_eye, m_lookAt, m_upVector);
}

void Camera::UpdateCamera(GLFWwindow *window, float xoffset, float yoffset, float deltaTime) {
  float sensitivity = 0.5f;
  float xAngle = xoffset * sensitivity * deltaTime;
  float yAngle = yoffset * sensitivity * deltaTime;

  glm::vec4 position(m_eye.x, m_eye.y, m_eye.z, 1);
  glm::vec4 pivot(m_lookAt.x, m_lookAt.y, m_lookAt.z, 1);

  float cosAngle = dot(GetViewDir(), m_upVector);
  if (cosAngle * (yAngle > 0 ? 1 : -1) > 0.99f) {
    yAngle = 0;
  }

  glm::mat4 rotationMatrixX = glm::rotate(glm::mat4(1.0f), xAngle, m_upVector);
  position = (rotationMatrixX * (position - pivot)) + pivot;

  glm::mat4 rotationMatrixY = glm::rotate(glm::mat4(1.0f), yAngle, GetRightVector());
  m_eye = (rotationMatrixY * (position - pivot)) + pivot;

  float zoomSpeed = 2.5f;
  glm::vec3 viewDir = GetViewDir();

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    m_eye += viewDir * zoomSpeed * deltaTime;
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    m_eye -= viewDir * zoomSpeed * deltaTime;
  }

  UpdateViewMatrix();
}

glm::mat4x4 Camera::GetViewMatrix() const {
  return m_viewMatrix;
}

glm::vec3 Camera::GetEye() const { return m_eye; }
glm::vec3 Camera::GetUpVector() const { return m_upVector; }
glm::vec3 Camera::GetLookAt() const { return m_lookAt; }

glm::vec3 Camera::GetViewDir() const {
  return glm::normalize(m_lookAt - m_eye);
}

glm::vec3 Camera::GetRightVector() const {
  return glm::normalize(glm::cross(GetViewDir(), m_upVector));
}
