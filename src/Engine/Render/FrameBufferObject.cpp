#include <BlackBox/Render/FrameBufferObject.hpp>
#include <BlackBox/Render/OpenglDebug.hpp>

#include <iostream>
using namespace std;

FrameBufferObject::FrameBufferObject(buffer_type type, int width, int height) : type(type), width(width), height(height)
{
}

bool FrameBufferObject::create()
{
  bool status = true;
  GLint internalFormat, Format;
  GLint filterMin, filterMag;
  GLint wrapS, wrapT;
  GLint dataType;
  GLint attachment;

  glCheck(glGenFramebuffers(1, &id));

  glCheck(glGenTextures(1, &texture));
  glCheck(glBindTexture(GL_TEXTURE_2D, texture));
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
    wrapS = wrapT = GL_REPEAT;
    dataType = GL_UNSIGNED_BYTE;
    break;
  case FrameBufferObject::HDR_BUFFER:
    internalFormat = GL_RGBA16F;
    Format = GL_RGBA;
    filterMin = filterMag = GL_LINEAR;
    wrapS = wrapT = GL_REPEAT;
    dataType = GL_FLOAT;
    break;
  default:
    break;
  }

  glCheck(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, Format, dataType, NULL));
  glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMin));
  glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMag));
  glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS));
  glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT));
  if (type == DEPTH_BUFFER)
  {
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
  }
  glCheck(glBindTexture(GL_TEXTURE_2D, 0));

  glCheck(glBindFramebuffer(GL_FRAMEBUFFER, id));
  if (type == SCENE_BUFFER || type == HDR_BUFFER)
  {
    glCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0));
    glCheck(glGenRenderbuffers(1, &rbo));
    glCheck(glBindRenderbuffer(GL_RENDERBUFFER, rbo));
    glCheck(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));
    glCheck(glBindRenderbuffer(GL_RENDERBUFFER, 0));
    glCheck(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo));
  }
  else if (type == DEPTH_BUFFER)
  {
    glCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0));
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
  }

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    status = false;
  }
  glCheck(glBindFramebuffer(GL_FRAMEBUFFER, 0));

  return status;
}

void FrameBufferObject::bind()
{
  glCheck(glBindFramebuffer(GL_FRAMEBUFFER, id));
}

void FrameBufferObject::unbind()
{
  glCheck(glBindFramebuffer(GL_FRAMEBUFFER, 0));
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
