#include "InputController.h"

void InputController::set_lastMousePos(glm::dvec2 &pos) {
  m_lastMousePos = pos;
}

void InputController::set_lastMousePosX(double posX) {
  m_lastMousePos.x = posX;
}

void InputController::set_lastMousePosY(double posY) {
  m_lastMousePos.y = posY;
}
