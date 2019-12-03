#pragma once

// Std. Includes
#include <vector>
#include <iostream>

// GL Includes
#include <BlackBox/Render/OpenGL/Core.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <BlackBox/Object.hpp>
#include <BlackBox/ISystem.hpp>
#include <BlackBox/IConsole.hpp>

struct ICVar;

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods

// Default camera values
const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 0.1f;
const GLfloat SENSITIVTY = 0.25f;
const GLfloat ZOOM = 45.0f;

// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class CCamera
{
public:
  // Camera Attributes
  Transform transform;
  glm::vec3 Front;
  glm::vec3 Up;
  glm::vec3 Right;
  glm::vec3 WorldUp;
  // Camera options
  ICVar* MovementSpeed;
  ICVar* FOV;
  GLfloat Zoom;
  //GLfloat FOV = 45.0f;
  GLfloat Ratio = 16.0f / 9;
  GLfloat zNear = 0.1f;
  //GLfloat zFar = 5000.f;
  ICVar* zFar = nullptr;

  enum class Mode
  {
    FPS,
    FLY
  }mode = Mode::FPS;

  // Constructor with vectors
  CCamera(glm::vec3 position = glm::vec3(0.0f, 3.0f, 5.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH);
  // Constructor with scalar values
  CCamera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch);

  // Returns the view matrix calculated using Eular Angles and the LookAt Matrix
  glm::mat4 getViewMatrix();
  glm::mat4 getProjectionMatrix();

  glm::vec3 getPosition();
  glm::vec3 getRotation();

  void setPosition(glm::vec3 pos);
  void setRotation(glm::vec3 ang);

  // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
  void ProcessKeyboard(Movement direction, GLfloat deltaTime);

  // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
  void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);

  // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
  void ProcessMouseScroll(GLfloat yoffset);

  // Calculates the front vector from the Camera's (updated) Eular Angles
  void updateCameraVectors();
};