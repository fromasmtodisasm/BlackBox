#pragma once
#ifdef GLAD_LOADER
//#define _WINDOWS_
#pragma warning(push)
#pragma warning(disable : 4005)
#include <glad/glad.h>
#pragma warning(pop)
#else
#ifdef GLEW_LOADER
#include <GL/glew.h>
#else
#error OPENGL LOADER NOT SETTED
#endif
#endif

#include <BlackBox/Render/OpenGL/Debug.hpp>

bool OpenGLLoader();

namespace gl {
	typedef Vec4 Color;

	inline void ViewPort(Vec4 viewPort)
	{
		glCheck(glViewport(static_cast<GLint>(viewPort.x), static_cast<GLint>(viewPort.x), static_cast<GLint>(viewPort.x), static_cast<GLint>(viewPort.x)));
	}

	// Framebuffer
	inline void BindFrameBuffer(GLuint id)
	{
		glCheck(glBindFramebuffer(GL_FRAMEBUFFER, id));
	}
	inline void FramebufferTexture2D(GLenum attachment, GLenum textarget, GLuint texture, GLint level)
	{
		glCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture, level));
	}

	inline void ClearColor(Color &color)
	{
		glCheck(glClearColor(color.r, color.g, color.b, color.a));
	}

	inline void Clear(GLbitfield mask)
	{
		glCheck(glClear(mask));
	}

	// Texture
	inline void TexParameteri(GLenum target, GLenum pname, GLint param)
	{
		glCheck(glTexParameteri(target, pname, param));
	}

	inline void BindTexture2D(GLuint texture)
	{
		glBindTexture(GL_TEXTURE_2D, texture);
	}

}
