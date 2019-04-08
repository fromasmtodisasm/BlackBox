#include <HackCamera.hpp>
#include <Opengl.hpp>
#include <glm/gtc/matrix_transform.hpp>

HackCamera::HackCamera(glm::vec3 pos)
{

}

HackCamera::HackCamera() : 
  m_pos(0,0,3), m_target(-glm::normalize(m_pos - glm::vec3(0,0,0))), m_right(1,0,0), m_up(0,1,0),
  m_angles(0,0,0)
{

}

HackCamera::~HackCamera()
{
}

void HackCamera::update()
{

}

void HackCamera::move(glm::vec3 pos)
{
  m_pos = m_pos + pos;
}

void HackCamera::strafe(glm::vec3 pos)
{

}

void HackCamera::rotateY(float angle)
{
  m_angles.y += angle;

  //Rotate viewdir around the up vector:
  m_target = glm::normalize(
    m_target*(float)cos(angle*PIdiv180)
    - m_right * (float)sin(angle*PIdiv180)
  );

  //now compute the new RightVector (by cross product)
  m_right = glm::cross(m_target, m_up);
}

void HackCamera::rotateX(float angle)
{
  m_angles.y += angle;
  //Rotate viewdir around the up vector:
  m_target = glm::normalize(
    m_target*(float)cos(angle*PIdiv180)
    + m_right * (float)sin(angle*PIdiv180)
  );
  //now compute the new RightVector (by cross product)
  m_right = -glm::cross(m_target, m_up);
}

void HackCamera::rotateZ(float angle)
{
  m_angles.y += angle;
  //Rotate viewdir around the up vector:
  m_right = glm::normalize(
    m_right*(float)cos(angle*PIdiv180)
    + m_up * (float)sin(angle*PIdiv180)
  );
  //now compute the new RightVector (by cross product)
  m_up = -glm::cross(m_target, m_right);
}

glm::mat4 HackCamera::getViewMatrix()
{
  return glm::lookAt(
    glm::vec3(m_pos),
    glm::vec3(m_pos + m_target),
    glm::vec3(m_up)
  );
}

glm::mat4 HackCamera::getProjectionMatrix()
{
  return glm::perspective(45.0f, 16/9.0f, 0.1f, 100.0f);
}

bool HackCamera::OnInputEvent(sf::Event & event)
{
  switch (event.type)
  {
  case sf::Event::KeyPressed:
    switch (event.key.code)
    {
    case sf::Keyboard::Up:
      move(m_target*(MOVE_SPEED));
      return true;
    case sf::Keyboard::Down:
      move(m_target*(-MOVE_SPEED));
      return true;
    case sf::Keyboard::A:
      move(m_right*(-MOVE_SPEED));
      return true;
    case sf::Keyboard::D:
      move(m_right*(MOVE_SPEED));
      return true;
    case sf::Keyboard::W:
      move(m_up*(MOVE_SPEED));
      return true;
    case sf::Keyboard::S:
      move(m_up*(-MOVE_SPEED));
      return true;
    case sf::Keyboard::Left:
      rotateY(1.0f);
      return true;
    case sf::Keyboard::Right:
      rotateY(-1.0f);
      return true;
    }
    break;
  case sf::Event::MouseLeft:
    break;
  }

  return false;
}
