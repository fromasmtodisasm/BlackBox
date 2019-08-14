#pragma once
#include <BlackBox/Render/Opengl.hpp>

struct ITexture;
class Texture;

class FrameBufferObject
{
public:
  enum BufferType
  {
    DEPTH_BUFFER,
    SCENE_BUFFER,
    HDR_BUFFER,
  };


  static FrameBufferObject *create(BufferType type, int width, int height, int attachment);
  void bind();
  void unbind();
  ITexture* getTexture();
private:
  FrameBufferObject(BufferType type, int width, int height, int attachment);
  void createSceneBuffer();
  void createDepthBuffer();
public:
  GLuint id;
  GLuint texture;
  GLuint rbo;
  int width;
  int height;
  BufferType type;
};
