#include <BlackBox/CCamera.hpp>
#include <BlackBox/Opengl.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp> 
#include <iostream>
#include <sstream>
using namespace  std;

CCamera::CCamera() :
  m_pos(0,0,3), m_target(0,0,-1), m_right(1,0,0), m_up(0,1,0),
  m_angles(0,0,0), m_view(1.0f)
{

}

CCamera::CCamera(glm::vec3 pos, glm::vec3 target, glm::vec3 up) :
  m_pos(pos), m_target(target), m_up(up),
  m_view(1.0f),m_angles(0,0,0), m_right(1,0,0)
{
}

CCamera::~CCamera()
{
}

void CCamera::update(float deltatime)
{
  /*
  static float prev_time;
  static stringstream ss;
  prev_time += deltatime;
  if (prev_time >= 1.0)
    cout << "cam.x = " << m_pos.x <<endl <<
         "cam.y = " << m_pos.y <<endl <<
         "cam.z = " << m_pos.z << endl;
  */


}

void CCamera::move(glm::vec3 pos)
{
  m_pos = m_pos + pos;
}

void CCamera::moveForward(float speed)
{
  move(m_target*(speed));
}

void CCamera::moveLeft(float speed)
{
  move(glm::normalize(m_right)*(-speed));
}

void CCamera::moveRight(float speed)
{
  move(glm::normalize(m_right)*(speed));
}

void CCamera::moveBackward(float speed)
{
  move(m_target*(-speed));
}

void CCamera::moveUp(float speed)
{
  move(m_up*(speed));
}

void CCamera::moveDown(float speed)
{
  move(m_up*(-speed));
}

void CCamera::strafeLeft(float speed)
{
  move(glm::normalize(m_right)*speed);
}

void CCamera::strafeRight(float speed)
{
  move(glm::normalize(m_right)*(-speed));
}

void CCamera::rotateY(float angle)
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

void CCamera::rotateX(float angle)
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

void CCamera::rotateZ(float angle)
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

void CCamera::rotateAroundTarget(float angle)
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

glm::mat4 CCamera::getViewMatrix()
{
  return glm::lookAt(
    glm::vec3(m_pos),
    glm::vec3(m_pos + m_target),
    glm::vec3(m_up)
  );
}

glm::mat4 CCamera::getProjectionMatrix()
{
  return glm::perspective(glm::radians(m_fov), m_ratio, 0.1f, 1000.0f);
}

void CCamera::reset()
{
  m_pos = {0,0,3}, 
  m_target = {-glm::normalize(m_pos - glm::vec3(0,0,0))}, 
  m_right = {1,0,0}, 
  m_up = {0,1,0}; 
}

void CCamera::setView(int x, int y, int w, int h)
{
  m_ratio = (float)w/h;
  glViewport(x,y,w,h);
}

bool CCamera::OnInputEvent(sf::Event & event)
{
  if (event.type == sf::Event::Resized)
  {
    setView(0,0,event.size.width, event.size.height);
  }
  return false;

}
