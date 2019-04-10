#pragma once
#include <InputHandler.hpp>
#include <glm/glm.hpp>
#include <set>
#define PI 3.1415926535897932384626433832795
#define PIdiv180 (PI/180.0)

class HackCamera : public IInputEventListener
{
private:
  const float MOVE_SPEED = 0.07f;
  glm::vec3 m_pos; 
  glm::vec3 m_target;
  glm::vec3 m_right;
  glm::vec3 m_up;
  glm::vec3 m_angles;
  glm::mat4 m_view;
  float m_ratio = 1.0f;
  float m_fov = 45.0f;
  float m_rotAngle = 0.9;
  bool isActive = true;
  std::set<sf::Keyboard::Key> m_keys;

public:
  HackCamera(glm::vec3 pos);
  HackCamera();
  ~HackCamera();

  void update(float deltatime);
  void move(glm::vec3 pos);
  void strafe(glm::vec3 pos);
  void rotateY(float angle);
  void rotateX(float angle);
  void rotateZ(float angle);
  void rotateAroundTarget(float angle);
  glm::mat4 getViewMatrix();
  glm::mat4 getProjectionMatrix();
  void reset();
  void setView(int x, int y, int w, int h);

  bool OnInputEvent(sf::Event &event);


};
