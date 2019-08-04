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

  glCheck(glGenFramebuffers(1, &id));

  glCheck(glGenTextures(1, &texture));
  glCheck(glBindTexture(GL_TEXTURE_2D, texture));
  if (type == SCENE_BUFFER)
  {
    glCheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
  }
  else if (type == DEPTH_BUFFER)
  {
    glCheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  }
  glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
  glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
  glCheck(glBindTexture(GL_TEXTURE_2D, 0));

  glCheck(glBindFramebuffer(GL_FRAMEBUFFER, id));
  if (type == SCENE_BUFFER)
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
