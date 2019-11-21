#include <BlackBox/Camera.hpp>
#include <BlackBox/Render/OpenGL/Core.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <BlackBox/IConsole.hpp>
#include <iostream>
#include <sstream>
using namespace  std;

// Constructor with vectors

CCamera::CCamera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(nullptr), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
{
  this->transform.position = position;
  this->WorldUp = up;
  this->transform.rotation.y = yaw;
  this->transform.rotation.x = pitch;
  this->updateCameraVectors();

  MovementSpeed = CREATE_CVAR("cam_speed", 5.0f, 0, "Speed of camera");
  FOV = CREATE_CVAR("fov", 45.0f, 0, "Camera field of view");
  zFar = CREATE_CVAR("zfar", 10000.f, 0, "Draw distance");

  REGISTER_CVAR(MouseSensitivity, 0.02f, 0);
}

// Constructor with scalar values

CCamera::CCamera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(nullptr), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
{
  this->transform.position = glm::vec3(posX, posY, posZ);
  this->WorldUp = glm::vec3(upX, upY, upZ);
  this->transform.rotation.y = yaw;
  this->transform.rotation.x = pitch;
  this->updateCameraVectors();
}

// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
glm::mat4 CCamera::getViewMatrix()
{
  return glm::lookAt(this->transform.position, this->transform.position + this->Front, this->Up);
}

glm::mat4 CCamera::getProjectionMatrix()
{
  return glm::perspective(glm::radians(FOV->GetFVal()), Ratio, zNear, zFar->GetFVal());
}

glm::vec3 CCamera::getPosition()
{
  return transform.position;
}

glm::vec3 CCamera::getRotation()
{
  return transform.rotation;
}

void CCamera::setPosition(glm::vec3 pos)
{
  transform.position = pos;
}

void CCamera::setRotation(glm::vec3 ang)
{
  transform.rotation = ang;
}

// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void CCamera::ProcessKeyboard(Movement direction, GLfloat deltaTime)
{
  GLfloat velocity = this->MovementSpeed->GetFVal() * deltaTime;
  if (direction == FORWARD)
    this->transform.position += glm::vec3(this->Front.x, mode == Mode::FPS ? 0 : this->Front.y, this->Front.z) * velocity;
  if (direction == BACKWARD)
    this->transform.position -= glm::vec3(this->Front.x, mode == Mode::FPS ? 0 : this->Front.y, this->Front.z) * velocity;
  if (direction == LEFT)
    this->transform.position -= this->Right * velocity;
  if (direction == RIGHT)
    this->transform.position += this->Right * velocity;
}

// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void CCamera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch)
{
  xoffset *= this->MouseSensitivity;
  yoffset *= this->MouseSensitivity;

  this->transform.rotation.y += xoffset;
  this->transform.rotation.x += yoffset;

  // Make sure that when pitch is out of bounds, screen doesn't get flipped
  if (constrainPitch)
  {
    if (this->transform.rotation.x > 89.0f)
      this->transform.rotation.x = 89.0f;
    if (this->transform.rotation.x < -89.0f)
      this->transform.rotation.x = -89.0f;
  }

  // Update Front, Right and Up Vectors using the updated Eular angles
  this->updateCameraVectors();
}

// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void CCamera::ProcessMouseScroll(GLfloat yoffset)
{
  if (this->Zoom >= 1.0f && this->Zoom <= 45.0f)
    this->Zoom -= yoffset;
  if (this->Zoom <= 1.0f)
    this->Zoom = 1.0f;
  if (this->Zoom >= 45.0f)
    this->Zoom = 45.0f;
}

// Calculates the front vector from the Camera's (updated) Eular Angles
void CCamera::updateCameraVectors()
{
  // Calculate the new Front vector
  glm::vec3 front;
  front.x = cos(glm::radians(this->transform.rotation.y)) * cos(glm::radians(this->transform.rotation.x));
  front.y = sin(glm::radians(this->transform.rotation.x));
  front.z = sin(glm::radians(this->transform.rotation.y)) * cos(glm::radians(this->transform.rotation.x));
  this->Front = glm::normalize(front);
  // Also re-calculate the Right and Up vector
  this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
  this->Up = glm::normalize(glm::cross(this->Right, this->Front));
}