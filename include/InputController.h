#ifndef INPUT_CONTROLLER_H
#define INPUT_CONTROLLER_H

#include "CameraController.h"
#include "Window.h"

class InputController {
public:
  InputController(Window &window, Camera &camera);

  void processInput(float deltaTime);

private:
  Window &m_window;
  Camera &m_camera;

  bool m_firstMouse = true;
  double m_lastX = 0.0;
  double m_lastY = 0.0;
};

#endif // INPUT_CONTROLLER_H
