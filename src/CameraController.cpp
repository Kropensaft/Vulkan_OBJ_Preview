#include "CameraController.h"
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

void Camera::UpdateCamera(const VkViewport &viewport) {
  // For now, let's implement a simple rotating camera for testing
  static float time = 0.0f;
  time += 0.01f; // Small increment for smooth rotation

  // Simple orbit around the origin
  float radius = 5.0f;
  float camX = sin(time) * radius;
  float camZ = cos(time) * radius;

  m_eye = glm::vec3(camX, 2.0f, camZ);
  m_lookAt = glm::vec3(0.0f, 0.0f, 0.0f);
  m_upVector = glm::vec3(0.0f, 1.0f, 0.0f);

  UpdateViewMatrix();

  // Print camera position every 60 frames for debugging
  static int frameCount = 0;
  if (frameCount++ % 60 == 0) {
    printf("Camera position: (%.2f, %.2f, %.2f)\n", m_eye.x, m_eye.y, m_eye.z);
  }
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
