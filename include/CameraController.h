/**
 * @file CameraController.h
 * @brief Camera system for 3D navigation.
 */
#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vulkan/vulkan_core.h>

/**
 * @class Camera
 * @brief Class representing a 3D camera and its controller.
 * @details Manages the view matrix, position, orientation, and handles user
 * input for camera movement and zooming.
 */
class Camera {
public:
  Camera() = default;

  /**
   * @brief Constructor initializing camera vectors.
   * @param eye Initial eye position.
   * @param lookat Target to look at.
   * @param upVector Up direction vector.
   */
  Camera(glm::vec3 &&eye, glm::vec3 &&lookat, glm::vec3 &&upVector);

  /** @brief Retrieves the computed view matrix. */
  glm::mat4x4 GetViewMatrix() const;

  /** @brief Retrieves the current position of the camera. */
  glm::vec3 GetEye() const;

  /** @brief Retrieves the camera's up vector. */
  glm::vec3 GetUpVector() const;

  /** @brief Retrieves the point the camera is looking at. */
  glm::vec3 GetLookAt() const;

  /** @brief Computes the normalized view direction vector. */
  glm::vec3 GetViewDir() const;

  /** @brief Computes the camera's right vector. */
  glm::vec3 GetRightVector() const;

  /** @brief Explicitly sets the camera's view parameters. */
  void SetCameraView(glm::vec3, glm::vec3, glm::vec3);

  /** @brief Recalculates the view matrix. */
  void UpdateViewMatrix();

  /** @brief Updates the camera state based on input. */
  void UpdateCamera(GLFWwindow *, float, float, float);

  /** @brief Initializes the camera's default state. */
  void CameraInit();

  /** @brief Callback function for handling mouse scroll events. */
  static void scrollCallback(GLFWwindow *, double, double);

  /** @brief Processes scroll input to adjust zoom distance. */
  void handleScroll(double);

  /** @brief Calculates a new position to zoom the camera out. */
  glm::vec3 zoomOut(float deltaTime);

  /** @brief Calculates a new position to zoom the camera in. */
  glm::vec3 zoomIn(float deltaTime);

  /** @brief Sets the sensitivity modifier for zoom operations. */
  inline void set_zoom_sensitivity(double);

  /** @brief Retrieves the current zoom sensitivity. */
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
  glm::vec3 m_eye;
  glm::vec3 m_lookAt;
  glm::vec3 m_upVector;
};

inline void Camera::set_zoom_sensitivity(double new_sens) {
  zoom_sensitivity = new_sens;
}
inline double Camera::get_zoom_sensitivity() const { return zoom_sensitivity; }

#endif // CAMERA_CONTROLLER_H
