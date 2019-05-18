#pragma once
#include <BlackBox/Opengl.hpp>

class Texture;

class FrameBufferObject
{
public:
  GLuint id;
  GLuint texture;
  GLuint rbo;
  int width;
  int height;

  FrameBufferObject(int width, int height);

  bool create();
  void bind();
  void unbind();
};
