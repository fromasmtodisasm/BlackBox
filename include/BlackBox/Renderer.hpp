#pragma once
#include <BlackBox/Opengl.hpp>
#include <BlackBox/CCamera.hpp>

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
