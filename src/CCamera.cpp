#include "..\include\CCamera.hpp"
#include "..\include\CCamera.hpp"
#include <CCamera.hpp>
#include <glm/gtc/matrix_transform.hpp>

CCamera::CCamera(glm::vec3 pos)
{

}

CCamera::CCamera()
{
}

CCamera::~CCamera()
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
    glm::vec3(0,5, -5 ), 
    glm::vec3( 0,0,0 ), 
    glm::vec3(0,1,0 )
  );
}

glm::mat4 CCamera::getProjectionMatrix()
{
  return glm::perspective(45.0f, 16/9.0f, 0.1f, 100.0f);
}

bool CCamera::OnInputEvent(sf::Event & event)
{
  return false;
}
