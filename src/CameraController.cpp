#include "CameraController.h"
#include "InputController.h"
#include "Storage.h"

void Camera::SetCameraView(glm::vec3 eye, glm::vec3 lookat, glm::vec3 up) {
  m_eye = std::move(eye);
  m_lookAt = std::move(lookat);
  m_upVector = std::move(up);
  UpdateViewMatrix();
}

void Camera::CameraInit() {
  float xPos = 100.0f;
  float yPos = 50.0f;
  float yDeltaAngle = 0.0f;
  glm::vec3 eye(0.0f, 0.0f, 5.0f);
  glm::vec3 lookat(0.0f, 0.0f, 0.0f);
  glm::vec3 up(0.0f, 1.0f, 0.0f);
  SetCameraView(eye, lookat, up);
}

void Camera::UpdateViewMatrix() {
  // Generate view matrix using the eye, lookAt and up vector
  m_viewMatrix = glm::lookAt(m_eye, m_lookAt, m_upVector);
}

Camera::Camera(glm::vec3 &&eye, glm::vec3 &&lookat, glm::vec3 &&upVector) : m_eye(std::move(eye)), m_lookAt(std::move(lookat)), m_upVector(std::move(upVector)) {
  UpdateViewMatrix();
}

// NOTE: Prepare the arcball camera's main Update function, algorithm is well known, we just need to implement the IO

void Camera::UpdateCamera(const VkViewport &viewport) {
  InputController IC;

  glm::vec4 position(GetEye().x, GetEye().y, GetEye().z, 1);
  glm::vec4 pivot(GetLookAt().x, GetLookAt().y, GetLookAt().z, 1);

  // VÝRAZNĚJŠÍ ÚHLY PRO TEST
  float deltaAngleX = (2 * M_PI / viewport.width) * 10.0f; // 10x citlivější
  float deltaAngleY = (M_PI / viewport.height) * 10.0f;    // 10x citlivější

  // Simulujte pohyb myši - kamera se bude pomalu otáčet
  static float time = 0.0f;
  time += 0.016f; // Přibližně 60 FPS

  float mouseX = sin(time) * 100.0f;       // Osciluje mezi -100 a 100
  float mouseY = cos(time * 0.7f) * 50.0f; // Osciluje pomaleji

  float xAngle = (mouseX - xPos) * deltaAngleX;
  float yAngle = (mouseY - yPos) * deltaAngleY;

  float cosAngle = dot(GetViewDir(), m_upVector);
  if (cosAngle * sgn(yAngle) > 0.99f)
    yAngle = 0;

  glm::mat4x4 rotationMatrixX(1.0f);
  rotationMatrixX = glm::rotate(rotationMatrixX, xAngle, m_upVector);
  position = (rotationMatrixX * (position - pivot)) + pivot;

  glm::mat4x4 rotationMatrixY(1.0f);
  rotationMatrixY = glm::rotate(rotationMatrixY, yAngle, GetRightVector());
  glm::vec3 finalPosition = (rotationMatrixY * (position - pivot)) + pivot;

  SetCameraView(finalPosition, GetLookAt(), m_upVector);

  // Aktualizujte pozici pro příští frame
  xPos = mouseX;
  yPos = mouseY;

  // Výpis pro debug (můžete odstranit)
  static int frameCount = 0;
  if (frameCount++ % 60 == 0) {
    printf("Camera position: (%.2f, %.2f, %.2f)\n",
           finalPosition.x, finalPosition.y, finalPosition.z);
  }
}
glm::mat4x4 Camera::GetViewMatrix() const {
  return m_viewMatrix;
}

glm::vec3 Camera::GetEye() const { return m_eye; }
glm::vec3 Camera::GetUpVector() const { return m_upVector; }
glm::vec3 Camera::GetLookAt() const { return m_lookAt; }

glm::vec3 Camera::GetViewDir() const { return -glm::transpose(m_viewMatrix)[2]; }
glm::vec3 Camera::GetRightVector() const { return glm::transpose(m_viewMatrix)[0]; }
