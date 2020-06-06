#pragma once

// Std. Includes
#include <vector>
#include <iostream>

#include <BlackBox/Core/MathHelper.hpp>
#include <BlackBox/System/IConsole.hpp>
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/Renderer/IRender.hpp>

struct ICVar;

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 0.1f;
const float SENSITIVTY = 0.25f;
const float ZOOM = 45.0f;

// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class CCamera
{
public:
  enum class Mode
  {
    FPS,
    FLY
  }mode = CCamera::Mode::FPS;
  enum class Type
  {
    Perspective,
    Ortho
  }type = Type::Perspective;
public:
  // Camera Attributes
  Transform transform;
  Vec3 Front;
  Vec3 Up;
  Vec3 Right;
  Vec3 WorldUp;
  // Camera options
  float MovementSpeed;
  float Zoom;
  float FOV = 45.0f;
  float Ratio = 16.0f / 9;
  float zNear = 0.1f;
  float zFar = 1000.f;

  // Constructor with vectors
  CCamera(glm::vec3 position = glm::vec3(0.0f, 3.0f, 5.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(10.f), Zoom(ZOOM)
  {
    this->transform.position = position;
    this->WorldUp = up;
    this->transform.rotation.y = yaw;
    this->transform.rotation.x = pitch;
    this->updateCameraVectors();

  }
  // Constructor with scalar values
  CCamera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(10.f), Zoom(ZOOM)
  {
    this->transform.position = glm::vec3(posX, posY, posZ);
    this->WorldUp = glm::vec3(upX, upY, upZ);
    this->transform.rotation.y = yaw;
    this->transform.rotation.x = pitch;
    this->updateCameraVectors();
  }

  // Returns the view matrix calculated using Eular Angles and the LookAt Matrix
  // Returns the view matrix calculated using Eular Angles and the LookAt Matrix
  Mat4 GetViewMatrix() const
  {
    return glm::lookAt(this->transform.position, this->transform.position + this->Front, this->Up);
  }
  Mat4 getProjectionMatrix() const
  {
	  Vec4d v;
	  gEnv->pRenderer->GetViewport(&v.x, &v.y, &v.z, &v.w);
	  if (type == Type::Perspective)
	  {
		  return glm::perspective(glm::radians(FOV), (float)(v.z) / (float)(v.w), zNear, zFar);
    }
	  else
	  {
		  return glm::ortho(0.f, 160.f, 0.f, 90.f, 0.1f, 500.f);
	  }
  }

  Vec3 GetPos()
  {
    return transform.position;
  }
  Vec3 GetAngles()
  {
    return transform.rotation;
  }

  
  void SetPos(glm::vec3 pos)
  {
    transform.position = pos;
  }

  void SetAngles(glm::vec3 ang)
  {
    transform.rotation = ang;
  }

  // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
  // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
  void ProcessMouseScroll(float yoffset)
  {
    if (this->Zoom >= 1.0f && this->Zoom <= 45.0f)
      this->Zoom -= yoffset;
    if (this->Zoom <= 1.0f)
      this->Zoom = 1.0f;
    if (this->Zoom >= 45.0f)
      this->Zoom = 45.0f;
}

  // Calculates the front vector from the Camera's (updated) Eular Angles
  void updateCameraVectors()
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
};

class COrthoCamera : public CCamera
{

};
