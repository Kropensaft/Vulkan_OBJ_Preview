#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#define GLFW_INCLUDE_VULKAN
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

  void SetCameraView(glm::vec3, glm::vec3, glm::vec3);
  void UpdateViewMatrix();

  void UpdateCamera(GLFWwindow *, float, float, float);
  void CameraInit();
  static void scrollCallback(GLFWwindow *, double, double);
  void handleScroll(double);
  glm::vec3 zoomOut(float deltaTime);
  glm::vec3 zoomIn(float deltaTime);

  inline void set_zoom_sensitivity(double);
  inline double get_zoom_sensitivity() const;

private:
  double zoom_sensitivity = 10.0;

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

void Camera::set_zoom_sensitivity(double new_sens) {
  zoom_sensitivity = new_sens;
}
double Camera::get_zoom_sensitivity() const { return zoom_sensitivity; }

#endif // CAMERA_CONTROLLER_H
