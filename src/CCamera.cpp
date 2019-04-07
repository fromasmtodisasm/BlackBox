#include <CCamera.hpp>
#include <Opengl.hpp>
#include <glm/gtc/matrix_transform.hpp>

CCamera::CCamera(glm::vec3 pos)
{

}

CCamera::CCamera() : m_pos(0,5,-5), m_target(0,0,0), m_up(0,1,0)
{

}

CCamera::~CCamera()
{
}

void CCamera::update()
{

}

void CCamera::move(glm::vec3 pos)
{
}

void CCamera::rotate(float angle, glm::vec3 axis)
{
}

glm::mat4 CCamera::getViewMatrix()
{
  return glm::lookAt(
    glm::vec3(m_pos),
    glm::vec3(m_target),
    glm::vec3(m_up)
  );
}

glm::mat4 CCamera::getProjectionMatrix()
{
  return glm::perspective(45.0f, 16/9.0f, 0.1f, 100.0f);
}

bool CCamera::OnInputEvent(sf::Event & event)
{
  if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
  {
    m_pos.x += 0.5f;
  }
  return false;
}
