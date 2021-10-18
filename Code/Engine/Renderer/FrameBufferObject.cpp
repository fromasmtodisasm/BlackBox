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
	glCheck(glCreateFramebuffers(1, &fbo->id));

	fbo->viewPort.z = (float)width;
	fbo->viewPort.w = (float)height;

	if (attachment->type != TextureType::DEPTH)
	{
		glCheck(glNamedFramebufferTexture(fbo->id, GL_COLOR_ATTACHMENT0, attachment->getId(), 0));
		glCheck(glCreateRenderbuffers(1, &fbo->rbo));
		glCheck(glBindRenderbuffer(GL_RENDERBUFFER, fbo->rbo));
		int samples = 0;
		if (attachment->isMS)
			samples = gEnv->pConsole->GetCVar("r_MSAA_samples")->GetIVal();
		glCheck(glNamedRenderbufferStorageMultisample(fbo->rbo, samples, GL_DEPTH_STENCIL, width, height));
		glCheck(glNamedFramebufferRenderbuffer(fbo->id, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, fbo->rbo));
		glCheck(glBindRenderbuffer(GL_RENDERBUFFER, 0));
		GLenum db = GL_COLOR_ATTACHMENT0;
		glCheck(glNamedFramebufferDrawBuffers(fbo->id, 1, &db));
	}
	else
	{
		gl::FramebufferTexture2D(GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, fbo->texture[0], 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}

	if (auto status = glCheckNamedFramebufferStatus(fbo->id, GL_FRAMEBUFFER); status != GL_FRAMEBUFFER_COMPLETE)
	{
		switch (status)
		{
		case GL_FRAMEBUFFER_UNDEFINED:
			gEnv->pLog->LogError("GL_FRAMEBUFFER_UNDEFINED");
			break;
		case GL_FRAMEBUFFER_COMPLETE:
			gEnv->pLog->LogError("GL_FRAMEBUFFER_COMPLETE");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			gEnv->pLog->LogError("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			gEnv->pLog->LogError("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			gEnv->pLog->LogError("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
			gEnv->pLog->LogError("GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS");
			break;
		default:
			gEnv->pLog->LogError("Undefined framebuffer status");
			break;
		}
		gEnv->pLog->LogError("Framebuffer not complete");
        //assert(0);
	}
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
	auto attachmen_type = texture->isMS ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	gl::FramebufferTexture2D(GL_COLOR_ATTACHMENT0, attachmen_type, texture->getId(), 0);
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

void FrameBufferObject::DrawToBackbuffer(const Vec4& dstViewport)
{
	DrawTo(nullptr, dstViewport);
}

void FrameBufferObject::DrawTo(FrameBufferObject* target, const Vec4& dstViewport)
{
	const auto& dvp = Vec4d(dstViewport);
	auto& svp = target == nullptr ? dvp : Vec4d(viewPort);
	auto target_id = target == nullptr ? 0 : target->id;
	glCheck(glBlitNamedFramebuffer(id, target_id, svp.x, svp.y, svp.z, svp.w, dvp.x, dvp.y, dvp.z, dvp.w, GL_COLOR_BUFFER_BIT, target == nullptr ? GL_LINEAR : GL_NEAREST));
}

void FrameBufferObject::createSceneBuffer()
{
}

void FrameBufferObject::createDepthBuffer()
{
}
