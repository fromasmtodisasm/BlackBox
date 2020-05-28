#include <BlackBox/Renderer/FrameBufferObject.hpp>
#include <BlackBox/Renderer/Texture.hpp>

#include <iostream>
#include <cassert>
#include <algorithm>
using namespace std;

FrameBufferObject::FrameBufferObject(int width, int height)
  :
  id(-1),
  rbo(-1),
  viewPort(glm::vec4(0))
{
}

FrameBufferObject::~FrameBufferObject()
{
  glDeleteFramebuffers(1, &id);
}

FrameBufferObject* FrameBufferObject::create(int width, int height, Texture* attachment, bool createMipChain)
{
  FrameBufferObject* fbo = new FrameBufferObject(width, height);
  glCheck(glGenFramebuffers(1, &fbo->id));

  fbo->viewPort.z = (float)width;
  fbo->viewPort.w = (float)height;

  gl::BindFramebuffer(fbo->id);

  if (attachment->type != TextureType::DEPTH)
  {
		gl::FramebufferTexture2D(GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, attachment->getId(), 0);
    glCheck(glGenRenderbuffers(1, &fbo->rbo));
    glCheck(glBindRenderbuffer(GL_RENDERBUFFER, fbo->rbo));
    glCheck(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));
    glCheck(glBindRenderbuffer(GL_RENDERBUFFER, 0));
    glCheck(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, fbo->rbo));
		GLenum db = GL_COLOR_ATTACHMENT0;
		glCheck(glDrawBuffers(1, &db));
  }
  else
  {
    gl::FramebufferTexture2D(GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, fbo->texture[0], 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
  }

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		gEnv->pLog->LogError("Framebuffer not complete");
    assert(0);
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

void FrameBufferObject::attach(Texture* texture)
{
	gl::FramebufferTexture2D(GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->getId(), 0);
}

void FrameBufferObject::bind()
{
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
	{
		bind(viewPort);
  }
	else
	{
		gEnv->pLog->LogError("Framebuffer not complete");
	}
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