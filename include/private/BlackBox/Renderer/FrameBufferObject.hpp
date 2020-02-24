#pragma once
#include <BlackBox/Renderer/OpenGL/Core.hpp>
#include <BlackBox/Core/MathHelper.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <vector>

#define EXTRACT_COLOR(c) c.r, c.g, c.b, c.a

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
  ~FrameBufferObject();

  static FrameBufferObject* create(BufferType type, int width, int height, int nColors, bool createMipChain);
  void clear(gl::Color const& color);
  void clear();
  void bind();
  void bind(glm::vec4 viewPort);
  static void bindDefault(glm::vec4 viewPort);
  void unbind();
  ITexture* getTexture();
private:
  FrameBufferObject(BufferType type, int width, int height, int nColors);
  void createSceneBuffer();
  void createDepthBuffer();
public:
  uint id;
  std::vector<uint> texture;
  uint rbo;
  BufferType type;
  glm::vec4 viewPort;
  glm::vec4 defaultColor = glm::vec4(glm::vec3(0.f), 1.f);
};