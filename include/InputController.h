/**
 * @file InputController.h
 * @brief User input processing.
 */
#ifndef INPUT_CONTROLLER_H
#define INPUT_CONTROLLER_H

#include "CameraController.h"
#include "Window.h"

/**
 * @class InputController
 * @brief Handles user input and bridges it to the camera and application state.
 */
class InputController {
public:
  /**
   * @brief Constructor for the InputController.
   * @param window Reference to the application Window object.
   * @param camera Reference to the Camera object to be controlled.
   */
  InputController(Window &window, Camera &camera);

  /**
   * @brief Processes active inputs for the current frame.
   * @param deltaTime The time elapsed since the last frame.
   */
  void processInput(float deltaTime);

private:
  Window &m_window;
  Camera &m_camera;

  bool m_firstMouse = true;
  double m_lastX = 0.0;
  double m_lastY = 0.0;
};

#endif // INPUT_CONTROLLER_H
