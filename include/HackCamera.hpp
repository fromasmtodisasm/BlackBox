#pragma once
#include <InputHandler.hpp>
#include <glm/glm.hpp>
#define PI 3.1415926535897932384626433832795
#define PIdiv180 (PI/180.0)

class HackCamera : public IInputEventListener
{
private:
  const float MOVE_SPEED = 0.50f;
  glm::vec3 m_pos; 
  glm::vec3 m_target;
  glm::vec3 m_right;
  glm::vec3 m_up;
  glm::vec3 m_angles;

public:
  HackCamera(glm::vec3 pos);
  HackCamera();
  ~HackCamera();

  void update();
  void move(glm::vec3 pos);
  void strafe(glm::vec3 pos);
  void rotateY(float angle);
  void rotateX(float angle);
  void rotateZ(float angle);
  glm::mat4 getViewMatrix();
  glm::mat4 getProjectionMatrix();

  bool OnInputEvent(sf::Event &event);


};
