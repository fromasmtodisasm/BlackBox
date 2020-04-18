#pragma once

// Std. Includes
#include <vector>
#include <iostream>

#include <BlackBox/Core/MathHelper.hpp>
#include <BlackBox/Renderer/Object.hpp>
#include <BlackBox/System/IConsole.hpp>

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
  // Camera Attributes
  Transform transform;
  Vec3 Front;
  Vec3 Up;
  Vec3 Right;
  Vec3 WorldUp;
  // Camera options
  ICVar* MovementSpeed;
  ICVar* FOV;
  float Zoom;
  //float FOV = 45.0f;
  float Ratio = 16.0f / 9;
  float zNear = 0.1f;
  //float zFar = 5000.f;
  ICVar* zFar = nullptr;

  enum class Mode
  {
    FPS,
    FLY
  }mode = Mode::FPS;

  // Constructor with vectors
  CCamera(glm::vec3 position = glm::vec3(0.0f, 3.0f, 5.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
  // Constructor with scalar values
  CCamera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

  // Returns the view matrix calculated using Eular Angles and the LookAt Matrix
  Mat4 getViewMatrix() const;
  Mat4 getProjectionMatrix() const;

  Vec3 getPosition();
  Vec3 getRotation();

  void setPosition(Vec3 pos);
  void setRotation(Vec3 ang);

  // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
  void ProcessKeyboard(Movement direction, float deltaTime);

  // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
  void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

  // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
  void ProcessMouseScroll(float yoffset);

  // Calculates the front vector from the Camera's (updated) Eular Angles
  void updateCameraVectors();
};