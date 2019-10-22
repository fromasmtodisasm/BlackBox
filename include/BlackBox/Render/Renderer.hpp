#pragma once
#include <BlackBox/Render/OpenGL/Core.hpp>
#include <BlackBox/Camera.hpp>

#include <cstddef>

class VertexArrayObject;

class Renderer
{
  bool init();
  void beginFrame();
  void endFrame();

  void setCamera(CCamera *camera);
  void drawVertexBuffer(VertexArrayObject *vao);
};
