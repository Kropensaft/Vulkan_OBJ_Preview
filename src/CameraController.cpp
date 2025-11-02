#include "CameraController.h"

void Camera::SetCameraView(glm::vec3 eye, glm::vec3 lookat, glm::vec3 up) {
  m_eye = std::move(eye);
  m_lookAt = std::move(lookat);
  m_upVector = std::move(up);
  UpdateViewMatrix();
}

void Camera::UpdateViewMatrix() {
  // Generate view matrix using the eye, lookAt and up vector
  m_viewMatrix = glm::lookAt(m_eye, m_lookAt, m_upVector);
}

Camera::Camera(glm::vec3 &&eye, glm::vec3 &&lookat, glm::vec3 &&upVector) : m_eye(std::move(eye)), m_lookAt(std::move(lookat)), m_upVector(std::move(upVector)) {
  UpdateViewMatrix();
}

/*NOTE: Prepare the arcball camera's main Update function, algorithm is well known, we just need to implement the IO

void Camera::UpdateCamera() {
  glm::vec4 position(GetEye().x, GetEye().y, GetEye().z, 1);
  glm::vec4 pivot(GetLookAt().x, GetLookAt().y, GetLookAt().z, 1);

  float deltaAngleX = (2 * M_PI / viewportWidth);
  float deltaAngleY = (M_PI / viewportHeight);
  float xAngle = (m_lastMousePos.x - xPos) * deltaAngleX;
  float yAngle = (m_lastMousePos.y - yPos) * deltaAngleY;

  float cosAngle = dot(GetViewDir(), m_upVector);
  if (cosAngle * sgn(yDeltaAngle) > 0.99f)
    yDeltaAngle = 0;

  glm::mat4x4 rotationMatrixX(1.0f);
  rotationMatrixX = glm::rotate(rotationMatrixX, xAngle, m_upVector);
  position = (rotationMatrixX * (position - pivot)) + pivot;

  glm::mat4x4 rotationMatrixY(1.0f);
  rotationMatrixY = glm::rotate(rotationMatrixY, yAngle, GetRightVector());
  glm::vec3 finalPosition = (rotationMatrixY * (position - pivot)) + pivot;

  SetCameraView(finalPosition, GetLookAt(), m_upVector);

  m_lastMousePos.x = xPos;
  m_lastMousePos.y = yPos;
}
*/
glm::mat4x4 Camera::GetViewMatrix() const {
  return m_viewMatrix;
}

glm::vec3 Camera::GetEye() const { return m_eye; }
glm::vec3 Camera::GetUpVector() const { return m_upVector; }
glm::vec3 Camera::GetLookAt() const { return m_lookAt; }

glm::vec3 Camera::GetViewDir() const { return -glm::transpose(m_viewMatrix)[2]; }
glm::vec3 Camera::GetRightVector() const { return glm::transpose(m_viewMatrix)[0]; }
