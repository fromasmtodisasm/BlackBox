#pragma once
#include <InputHandler.hpp>
#include <glm/glm.hpp>

class CCamera : public IInputEventListener
{
private:
  glm::vec3 m_pos; 
  glm::vec3 m_target;
  glm::vec3 m_up;

public:
  CCamera(glm::vec3 pos);
  CCamera();
  ~CCamera();

  void update();
  void move(glm::vec3 pos);
  void rotate(float angle, glm::vec3 axis);
  glm::mat4 getViewMatrix();
  glm::mat4 getProjectionMatrix();

  bool OnInputEvent(sf::Event &event);


};
