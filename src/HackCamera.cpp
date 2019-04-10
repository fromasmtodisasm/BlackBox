#include <HackCamera.hpp>
#include <Opengl.hpp>
#include <glm/gtc/matrix_transform.hpp>

HackCamera::HackCamera(glm::vec3 pos)
{

}

HackCamera::HackCamera() : 
  m_pos(0,0,3), m_target(0,0,-1), m_right(1,0,0), m_up(0,1,0),
  m_angles(0,0,0), m_view(1.0f)
{

}

HackCamera::~HackCamera()
{
}

void HackCamera::update(float deltatime)
{
  float speed = deltatime*MOVE_SPEED;
  float rotSpeed = deltatime*m_rotAngle; 
  for (const auto &key : m_keys)
  {
    switch (key)
    {
    case sf::Keyboard::W:
      move(m_target*(speed));
      break;
    case sf::Keyboard::S:
      move(m_target*(-speed));
      break;
    case sf::Keyboard::A:
      move(glm::normalize(m_right)*(-speed));
      break;
    case sf::Keyboard::D:
      move(glm::normalize(m_right)*(speed));
      break;
    case sf::Keyboard::Up:
      move(m_up*(speed));
      break;
    case sf::Keyboard::Down:
      move(m_up*(-speed));
      break;
    case sf::Keyboard::Left:
      rotateY(rotSpeed);
      break;
    case sf::Keyboard::Right:
      rotateY(-rotSpeed);
      break;
    case sf::Keyboard::N:
      rotateZ(rotSpeed);
      break;
    case sf::Keyboard::M:
      rotateZ(-rotSpeed);
      break;
    case sf::Keyboard::F:
      rotateX(rotSpeed);
      break;
    case sf::Keyboard::G:
      rotateX(-rotSpeed);
      break;
    case sf::Keyboard::R:
      reset();
      break;
    case sf::Keyboard::T:
      rotateAroundTarget(rotSpeed);
      break;
    case sf::Keyboard::Num1:
      rotateY(180.0f);
      break;
    case sf::Keyboard::Num2:
      m_fov += 0.5f;
      break;
    case sf::Keyboard::Num3:
      m_fov -= 0.5f;
      break;
    }
  }
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
  m_up = glm::normalize(
    m_up*(float)cos(angle*PIdiv180)
    + m_target * (float)sin(angle*PIdiv180)
  );
  //now compute the new RightVector (by cross product)
  m_target = -glm::cross(m_right, m_up);
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

void HackCamera::rotateAroundTarget(float angle)
{
  this->m_angles.y += angle;
  //m_pos.x = (float)sin(m_angles.y)*10;
  //m_pos.z = (float)cos(m_angles.y)*10;

  m_pos.x = m_pos.x*(float)cos(angle*PIdiv180)
  - m_pos.z * (float)sin(angle*PIdiv180);
  m_pos.z = m_pos.x*(float)sin(angle*PIdiv180)
  + m_pos.z * (float)cos(angle*PIdiv180);
  //m_target = { 0,0,0 };

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
  return glm::perspective(glm::radians(m_fov), m_ratio, 0.1f, 100.0f);
}

void HackCamera::reset()
{
  m_pos = {0,0,3}, 
  m_target = {-glm::normalize(m_pos - glm::vec3(0,0,0))}, 
  m_right = {1,0,0}, 
  m_up = {0,1,0}; 
}

void HackCamera::setView(int x, int y, int w, int h)
{
  m_ratio = (float)w/h;
  glViewport(x,y,w,h);
}

bool HackCamera::OnInputEvent(sf::Event & event)
{
  switch (event.type)
  {
  case sf::Event::KeyPressed:
    m_keys.insert(event.key.code);
    return true;
  case sf::Event::KeyReleased:
    m_keys.erase(event.key.code);
    return true;
  case sf::Event::Resized:
    {
      //glViewport(0, 0, event.size.width, event.size.height);
      //setView(event.size.width, event.size.height);
    }
    return true;
  }
  case sf::Event::MouseMoved:
  {
    m_fov += event.mouseWheel.delta * 0.1;
    return true;
  }
  }
  return false;

}
