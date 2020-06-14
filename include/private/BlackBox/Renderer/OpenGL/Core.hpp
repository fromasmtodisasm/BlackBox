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
#pragma message(__FILE__ "OPENGL LOADER NOT SETTED")
//#error OPENGL LOADER NOT SETTED
#endif
#endif

#include <BlackBox/Renderer/OpenGL/Debug.hpp>
#include <BlackBox/Core/MathHelper.hpp>

#include <iostream>
#include <fstream>
#include <cstring>

#define TO_STR(v, m) case GL_##v: { return m"GL_"#v; }
#define GET_SOURCE(v) TO_STR(v, "Source: ")
#define GET_TYPE(v) TO_STR(v, "Type: ")
#define GET_SEVERITY(v) TO_STR(v, "Severity: ")
#define DEBUG_GROUP(message) CDebugSection debugSection(sizeof(message), message)

#if defined(_DEBUG) || defined(GL_DEBUG) || !defined(NDEBUG)

// In debug mode, perform a test on every OpenGL call
// The do-while loop is needed so that glCheck can be used as a single statement in if/else branches
#define glCheck(expr) do { expr; OpenglDebuger::checkError(__FILE__, __LINE__, #expr); } while (false)

#else

// Else, we don't add any overhead
#define glCheck(expr) do { expr; OpenglDebuger::checkError(__FILE__, __LINE__, #expr); } while (false)
//#define glCheck(expr) (expr)

#endif

class OpenglDebuger
{
public:
	OpenglDebuger() = default;
  OpenglDebuger(const char* file);
  ~OpenglDebuger();

  static inline void SetIgnore(bool v)
  {
    ignore = v;
  }
  static void checkError(const char* file, int line, const char* expr);

  static inline void PushGroup(GLuint id, GLsizei length, const char* message)
  {
    glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, id, length, message);
  }

  static inline void EndGroup()
  {
    glPopDebugGroup();
  }
private:
  static void APIENTRY callBack(GLenum source, GLenum type, GLuint id,
    GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

  static inline const char* SOURCE_TO_STRING(GLenum s)
  {
    switch (s)
    {
      GET_SOURCE(DEBUG_SOURCE_API);
      GET_SOURCE(DEBUG_SOURCE_WINDOW_SYSTEM);
      GET_SOURCE(DEBUG_SOURCE_SHADER_COMPILER);
      GET_SOURCE(DEBUG_SOURCE_THIRD_PARTY);
      GET_SOURCE(DEBUG_SOURCE_APPLICATION);
      GET_SOURCE(DEBUG_SOURCE_OTHER);
    default:
      return "Unknown source";
    }
  }

  static inline const char* TYPE_TO_STRING(GLenum t)
  {
    switch (t)
    {
      GET_TYPE(DEBUG_TYPE_ERROR);
      GET_TYPE(DEBUG_TYPE_DEPRECATED_BEHAVIOR);
      GET_TYPE(DEBUG_TYPE_UNDEFINED_BEHAVIOR);
      GET_TYPE(DEBUG_TYPE_PORTABILITY);
      GET_TYPE(DEBUG_TYPE_PERFORMANCE);
      GET_TYPE(DEBUG_TYPE_MARKER);
      GET_TYPE(DEBUG_TYPE_PUSH_GROUP);
      GET_TYPE(DEBUG_TYPE_POP_GROUP);
      GET_TYPE(DEBUG_TYPE_OTHER);
    default:
      return "Unknown type";
    }
  }

  static inline const char* SEVERITY_TO_STRING(GLenum s)
  {
    switch (s)
    {
      GET_SEVERITY(DEBUG_SEVERITY_HIGH);
      GET_SEVERITY(DEBUG_SEVERITY_MEDIUM);
      GET_SEVERITY(DEBUG_SEVERITY_LOW);
      GET_SEVERITY(DEBUG_SEVERITY_NOTIFICATION);
    default:
      return "Unknown severity";
    }
  }

private:
  std::ofstream debug_file;
  static bool isError;
  static bool ignore;
};

//////////////////////////////////////////////////////////////////////////
//! CFrameProfilerSection is an auto class placed where code block need to be profiled.
//! Every time this object is constructed and destruted the time between constructor
//! and destructur is merged into the referenced CFrameProfiler instance.
//!
class CDebugSection
{
public:

  inline CDebugSection(size_t length, const char* message)
  {
    OpenglDebuger::PushGroup(0, static_cast<GLsizei>(length), message);
  }
  inline ~CDebugSection()
  {
    OpenglDebuger::EndGroup();
  }
};

namespace debuger
{
  inline void object_label(GLenum id, GLuint object, const char* label)
  {
    glCheck(glObjectLabel(id, object, static_cast<GLsizei>(std::strlen(label)), label));
  }
  inline void object_label(GLenum id, GLuint object, std::string const& label)
  {
    glCheck(glObjectLabel(id, object, static_cast<GLsizei>(label.size()), label.c_str()));
  }

  template<typename T> inline void buffer_label(GLuint object, T const& label)
  {
    object_label(GL_BUFFER, object, label);
  }
  template<typename T> inline void shader_label(GLuint object, T const& label)
  {
    object_label(GL_SHADER, object, label);
  }
  template<typename T> inline void program_label(GLuint object, T const& label)
  {
    object_label(GL_PROGRAM, object, label);
  }
  // Vertex array
  template<typename T> inline void vertex_array_label(GLuint object, T const& label)
  {
    object_label(GL_VERTEX_ARRAY, object, label);
  }
  template<typename T> inline void query_label(GLuint object, T const& label)
  {
    object_label(GL_QUERY, object, label);
  }
  template<typename T> inline void pipeline_label(GLuint object, T const& label)
  {
    object_label(GL_PROGRAM_PIPELINE, object, label);
  }
  // Transform feedback
  template<typename T> inline void transform_feedback_label(GLuint object, T const& label)
  {
    object_label(GL_TRANSFORM_FEEDBACK, object, label);
  }
  template<typename T> inline void sampler_label(GLuint object, T const& label)
  {
    object_label(GL_SAMPLER, object, label);
  }
  template<typename T> inline void texture_label(GLuint object, T const& label)
  {
    object_label(GL_TEXTURE, object, label);
  }
  // Render buffer
  template<typename T> inline void render_buffer_label(GLuint object, T const& label)
  {
    object_label(GL_RENDERBUFFER, object, label);
  }
  template<typename T> inline void frame_buffer_label(GLuint object, T const& label)
  {
    object_label(GL_FRAMEBUFFER, object, label);
  }

  inline void Ignore(bool v) { OpenglDebuger::SetIgnore(v); }

  /*
  template<class T> using va_label = decltype(vertex_array_label<T>);
  template<class T> using tf_label = decltype(transform_feedback_label<T>);
  template<class T> using rb_label = decltype(render_buffer_label<T>);
  template<class T> using fb_label = decltype(frame_buffer_label<T>);
  */
}

bool OpenGLLoader();

namespace gl {
  typedef Vec4 Color;

  inline void Enable(GLenum cap)
  {
    glCheck(glEnable(cap));
  }

  inline void Disable(GLenum cap)
  {
    glCheck(glDisable(cap));
  }
  inline void CullFace(GLenum mode)
  {
    glCheck(glCullFace(mode));
  }

  inline void ViewPort(Vec4 const& viewPort)
  {
    glCheck(glViewport(static_cast<GLint>(viewPort.x), static_cast<GLint>(viewPort.y), static_cast<GLint>(viewPort.z), static_cast<GLint>(viewPort.w)));
  }

  inline void ViewPort(GLuint x, GLuint y, GLuint w, GLuint h)
  {
    glCheck(glViewport(x,y,w,h));
  }

  inline void Scissor(GLuint x, GLuint y, GLuint w, GLuint h)
  {
    glCheck(glScissor(x,y,w,h));
  }

  // Framebuffer
  inline void BindFramebuffer(GLuint id)
  {
    glCheck(glBindFramebuffer(GL_FRAMEBUFFER, id));
  }
  inline void BindDrawFramebuffer(GLuint id)
  {
    glCheck(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, id));
  }
  inline void BindReadFramebuffer(GLuint id)
  {
    glCheck(glBindFramebuffer(GL_READ_FRAMEBUFFER, id));
  }
  inline void DrawBackBuffer()
  {
    glCheck(glDrawBuffer(GL_BACK));
  }
  inline void BlitFrameBuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter)
  {
	  glCheck(glBlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter));
  }
  inline void FramebufferTexture2D(GLenum attachment, GLenum textarget, GLuint texture, GLint level)
  {
    glCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture, level));
  }

  inline void ClearColor(Color const& color)
  {
    glCheck(glClearColor(color.r, color.g, color.b, color.a));
  }

  inline void Clear(GLbitfield mask)
  {
    glCheck(glClear(mask));
  }

  // Texture
  inline void CreateTextures2D(GLenum target, GLsizei n, GLuint *textures)
  {
	  glCheck(glCreateTextures(target, n, textures));
  }

  inline void BindTexture(GLenum target, GLuint texture)
  {
	  glCheck(glBindTexture(target, texture));
  }
  inline void TextureImage2D(
      GLint texture, 
      GLint level, 
      GLint internalformat, 
      GLsizei width, 
      GLsizei height,
      GLint border,
      GLenum format,
      GLenum type,
      const void *pixels
  )
  {
    glCheck(glTextureSubImage2D(texture,
      level, internalformat, width, height, border, format, type, pixels
    ));
  }

  inline void TexImage2DMS(
    GLenum target,
		GLsizei samples,
		GLenum internalformat,
		GLsizei width,
		GLsizei height,
		GLboolean fixedsamplelocations)
  {
	  glCheck(glTexImage2DMultisample(target, samples, internalformat, width, height, fixedsamplelocations));
  }

  inline void TextureParameteri(GLint texture, GLenum pname, GLint param)
  {
    glCheck(glTextureParameteri(texture, pname, param));
  }

  inline void BindTexture2D(GLenum target, GLuint texture)
  {
    glCheck(glBindTexture(target, texture));
  }

  inline void ActiveTexture(GLenum texture)
  {
    glCheck(glActiveTexture(texture));
  }

  // VAO
  inline void GenVertexArrays(GLsizei n, GLuint *arrays)
  {
		glCheck(glGenVertexArrays(n, arrays));
  }
  inline void CreateVertexArrays(GLsizei n, GLuint *arrays)
  {
		glCheck(glCreateVertexArrays(n, arrays));
  }

  inline void BindVertexArray(GLuint array)
  {
		glCheck(glBindVertexArray(array));
  }

  inline void DrawArrays(GLenum mode, GLint first, GLsizei count)
  {
		glCheck(glDrawArrays(mode, first, count));
  }

  inline void DrawElements(GLenum mode, GLsizei count, GLenum type, const void *indices)
  {
		glCheck(glDrawElements(mode, count, type, indices));
  }

  inline void EnableVertexAttribArray(GLuint index)
  {
    glCheck(glEnableVertexAttribArray(index));
  }

  inline void DisableVertexAttribArray(GLuint index)
  {
    glCheck(glDisableVertexAttribArray(index));
  }

  inline void VertexAttribPointer(GLuint index, GLint size, GLenum Type, GLboolean normalized, GLsizei stride, const void* pointer)
  {
    glCheck(glVertexAttribPointer(index, size, Type, normalized, stride, pointer));
  }

	inline void GenBuffers(GLsizei n, GLuint* buffers)
	{
		glCheck(glGenBuffers(n, buffers));
	}

	inline void GenBuffer(GLuint* buffers)
	{
		glCheck(glGenBuffers(1, buffers));
	}

	inline void BindBuffer(GLenum target, GLuint buffer)
	{
		glCheck(glBindBuffer(target, buffer));
	}

	inline void BufferData(GLenum target, GLsizeiptr size, const void *data, GLenum usage)
	{
		glCheck(glBufferData(target, size, data, usage));
	}

	inline void NamedBufferData(GLuint buffer, GLsizeiptr size, const void *data, GLenum usage)
	{
		glCheck(glNamedBufferData(buffer, size, data, usage));
	}

	inline void BufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const void *data)
	{
		glCheck(glBufferSubData(target, offset, size, data));
	}

	inline void NamedBufferSubData(GLint buffer, GLintptr offset, GLsizeiptr size, const void *data)
	{
		glCheck(glNamedBufferSubData(buffer, offset, size, data));
	}

	inline void DeleteBuffers(GLsizei n, const GLuint *buffers)
	{
		glCheck(glDeleteBuffers(n, buffers));
	}


  inline void UniformValue(GLint location, int value)
  {
    glCheck(glUniform1i(location, value));
  }

  inline void UniformValue(GLint location, unsigned int value)
  {
    glCheck(glUniform1ui(location, value));
  }

  inline void UniformValue(GLint location, float value)
  {
    glCheck(glUniform1f(location, value));
  }

  inline void UniformValue(GLint location, glm::vec1 const& value)
  {
    glCheck(glUniform1fv(location, 1, glm::value_ptr(value)));
  }

  inline void UniformValue(GLint location, glm::vec2 const& value)
  {
    glCheck(glUniform2fv(location, 1, glm::value_ptr(value)));
  }

  inline void UniformValue(GLint location, glm::vec3 const& value)
  {
    glCheck(glUniform3fv(location, 1, glm::value_ptr(value)));
  }

  inline void UniformValue(GLint location, glm::vec4 const& value)
  {
    glCheck(glUniform4fv(location, 1, glm::value_ptr(value)));
  }

  inline void UniformValue(GLint location, glm::mat2& value)
  {
    glCheck(glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(value)));
  }

  inline void UniformValue(GLint location, glm::mat3& value)
  {
    glCheck(glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value)));
  }

  inline void UniformValue(GLint location, glm::mat4& value)
  {
    glCheck(glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value)));
  }

  // Shader
  template<typename T>
  inline void Uniform(GLint location, T& value)
  {
    UniformValue(location, value);
  }

  // Get
  inline const GLubyte* GetString(GLenum name)
  {
    return glGetString(name);
  }

  inline void GetIntegerv(GLenum name, GLint *data)
  {
    glGetIntegerv(name, data);
  }

  inline void GetFloatv(GLenum name, GLfloat *data)
  {
    glGetFloatv(name, data);
  }
}
