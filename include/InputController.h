#ifndef INPUT_CONTROLLER_H
#define INPUT_CONTROLLER_H

#include "glm/glm.hpp"

class InputController {
public:
  InputController() = default;

  glm::dvec2 get_lastMousePos() const;
  void set_lastMousePos(glm::dvec2 &);
  void set_lastMousePosX(double);
  void set_lastMousePosY(double);

private:
  glm::dvec2 m_lastMousePos = {0, 0};
};

inline glm::dvec2 InputController::get_lastMousePos() const {
  return m_lastMousePos;
}
#endif // !INPUT_CONTROLLER_H
