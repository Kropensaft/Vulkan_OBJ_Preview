#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vulkan/vulkan_core.h>

class Camera {
public:
  Camera() = default;

  Camera(glm::vec3 &&eye, glm::vec3 &&lookat, glm::vec3 &&upVector);

  glm::mat4x4 GetViewMatrix() const;
  glm::vec3 GetEye() const;
  glm::vec3 GetUpVector() const;
  glm::vec3 GetLookAt() const;

  glm::vec3 GetViewDir() const;
  glm::vec3 GetRightVector() const;

  void SetCameraView(glm::vec3 eye, glm::vec3 lookat, glm::vec3 up);
  void UpdateViewMatrix();

  void UpdateCamera(const VkViewport &);
  void CameraInit();

private:
  float xPos;
  float yPos;
  float yDeltaAngle;

  float lastMouseX = 0.f;
  float lastMouseY = 0.f;
  bool firstMouse = false;

  glm::mat4x4 m_viewMatrix;
  glm::vec3 m_eye;      // INFO: Camera position in 3D
  glm::vec3 m_lookAt;   // Point that the camera is looking at
  glm::vec3 m_upVector; // Orientation of the camera
};

#endif // CAMERA_CONTROLLER_H
