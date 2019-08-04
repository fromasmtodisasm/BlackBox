#pragma once
#include <BlackBox/Render/Opengl.hpp>

struct ITexture;
class Texture;

class FrameBufferObject
{
public:
  enum buffer_type
  {
    DEPTH_BUFFER,
    SCENE_BUFFER
  }type = SCENE_BUFFER;
  GLuint id;
  GLuint texture;
  GLuint rbo;
  int width;
  int height;

  FrameBufferObject(buffer_type type, int width, int height);

  bool create();
  void bind();
  void unbind();
  ITexture* getTexture();
private:
  void createSceneBuffer();
  void createDepthBuffer();
};
