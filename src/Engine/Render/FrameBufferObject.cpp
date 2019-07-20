#include <BlackBox/Render/FrameBufferObject.hpp>
#include <BlackBox/Render/OpenglDebug.hpp>

#include <iostream>
using namespace std;

FrameBufferObject::FrameBufferObject(int width, int height) : width(width), height(height)
{
}

bool FrameBufferObject::create()
{
  bool status = true;

  glCheck(glGenFramebuffers(1, &id));
  glCheck(glBindFramebuffer(GL_FRAMEBUFFER, id));
  // создание текстурного объекта
  glCheck(glGenTextures(1, &texture));
  glCheck(glBindTexture(GL_TEXTURE_2D, texture));
  glCheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));
  glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
  glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
  glCheck(glBindTexture(GL_TEXTURE_2D, 0));

  // присоедиение текстуры к объекту текущего кадрового буфера
  glCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0));

  glCheck(glGenRenderbuffers(1, &rbo));
  glCheck(glBindRenderbuffer(GL_RENDERBUFFER, rbo));
  glCheck(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));
  glCheck(glBindRenderbuffer(GL_RENDERBUFFER, 0));

  glCheck(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo));

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
