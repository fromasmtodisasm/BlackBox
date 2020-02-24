#include <BlackBox/Renderer/FrameBufferObject.hpp>
#include <BlackBox/Renderer/OpenGL/Debug.hpp>

#include <iostream>
#include <cassert>
#include <algorithm>
using namespace std;

FrameBufferObject::FrameBufferObject(BufferType type, int width, int height, int nColors)
  :
  type(type),
  id(-1),
  rbo(-1),
  viewPort(glm::vec4(0))
{
}

FrameBufferObject::~FrameBufferObject()
{
  glDeleteFramebuffers(1, &id);
}

FrameBufferObject* FrameBufferObject::create(BufferType type, int width, int height, int nColors, bool createMipChain)
{
  bool status = true;
  GLint internalFormat, Format;
  GLint filterMin, filterMag;
  GLint wrapS, wrapT;
  GLint dataType;

  int texCnt = 1;
  int mip_cnt = 1;
  if (createMipChain)
  {
    if (nColors > 1)
      return nullptr;
    mip_cnt = static_cast<int>(std::log2(std::max(width, height)));
  }

  FrameBufferObject* fbo = new FrameBufferObject(type, width, height, nColors);
  glCheck(glGenFramebuffers(1, &fbo->id));

  fbo->viewPort.z = (float)width;
  fbo->viewPort.w = (float)height;

  if (type != DEPTH_BUFFER)
  {
    texCnt = nColors;
  }
  fbo->texture.resize(texCnt);
  glCheck(glGenTextures(nColors, &fbo->texture[0]));
  switch (type)
  {
  case FrameBufferObject::DEPTH_BUFFER:
    internalFormat = Format = GL_DEPTH_COMPONENT;
    filterMin = filterMag = GL_LINEAR;
    wrapS = wrapT = GL_CLAMP_TO_BORDER;
    dataType = GL_FLOAT;
    //attachment =
    break;
  case FrameBufferObject::SCENE_BUFFER:
    internalFormat = Format = GL_RGBA;
    filterMin = filterMag = GL_LINEAR;
    wrapS = wrapT = GL_CLAMP_TO_EDGE;
    dataType = GL_UNSIGNED_BYTE;
    break;
  case FrameBufferObject::HDR_BUFFER:
    internalFormat = GL_RGBA16F;
    Format = GL_RGBA;
    filterMin = filterMag = GL_LINEAR;
    wrapS = wrapT = GL_CLAMP_TO_EDGE;
    dataType = GL_FLOAT;
    break;
  default:
    break;
  }

  gl::BindFramebuffer(fbo->id);

  {
    for (int i = 0; i < texCnt; i++)
    {
      for (int j = 0, mw = width, mh = height; j < mip_cnt; j++)
      {
        gl::BindTexture2D(fbo->texture[i]);
        glCheck(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, mw, mh, 0, Format, dataType, NULL));
        gl::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMin);
        gl::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMag);
        gl::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
        gl::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
        float b[] = { 0,0,0,1 };
        glCheck(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, b));
        if (createMipChain)
        {
          mw /= 2;
          mh /= 2;
        }

        if (type != DEPTH_BUFFER)
          gl::FramebufferTexture2D(GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, fbo->texture[i], 0);
      }
    }
  }
  if (type == DEPTH_BUFFER)
  {
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
  }
  gl::BindTexture2D(0);

  if (type == SCENE_BUFFER || type == HDR_BUFFER)
  {
    glCheck(glGenRenderbuffers(1, &fbo->rbo));
    glCheck(glBindRenderbuffer(GL_RENDERBUFFER, fbo->rbo));
    glCheck(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));
    glCheck(glBindRenderbuffer(GL_RENDERBUFFER, 0));
    glCheck(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, fbo->rbo));
  }
  else if (type == DEPTH_BUFFER)
  {
    glCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, fbo->texture[0], 0));
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
  }

  if (texCnt > 1 && type != DEPTH_BUFFER)
  {
    // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
    std::vector<unsigned int> attachments;
    for (int i = 0; i < texCnt; i++)
    {
      attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
    }
    glCheck(glDrawBuffers(texCnt, &attachments[0]));
  }
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    assert(0);
    status = false;
  }
  gl::BindFramebuffer(0);

  return fbo;
}

void FrameBufferObject::clear(gl::Color const& color)
{
  gl::ClearColor(color);
  glCheck(glClearDepthf(1.f));
  gl::Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FrameBufferObject::clear()
{
  clear(defaultColor);
}

void FrameBufferObject::bind()
{
  bind(viewPort);
}

void FrameBufferObject::bind(glm::vec4 viewPort)
{
  gl::BindFramebuffer(id);
  gl::ViewPort(viewPort);
}

void FrameBufferObject::bindDefault(glm::vec4 viewPort)
{
  gl::BindFramebuffer(0);
  gl::ViewPort(viewPort);
}

void FrameBufferObject::unbind()
{
  gl::BindFramebuffer(0);
}

ITexture* FrameBufferObject::getTexture()
{
  return nullptr;
}

void FrameBufferObject::createSceneBuffer()
{
}

void FrameBufferObject::createDepthBuffer()
{
}