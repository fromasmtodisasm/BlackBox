#pragma once
#include <BlackBox/Render/Opengl.hpp>
#include <iostream>
#include <fstream>
#include <cstring>

#define TO_STR(v, m) case GL_##v: { return m"GL_"#v; }
#define GET_SOURCE(v) TO_STR(v, "Source: ")
#define GET_TYPE(v) TO_STR(v, "Type: ")
#define GET_SEVERITY(v) TO_STR(v, "Severity: ")
#define DEBUG_GROUP(message) CDebugSection debugSection(sizeof(message), message)

#if defined(_DEBUG) || defined(GL_DEBUG)

// In debug mode, perform a test on every OpenGL call
// The do-while loop is needed so that glCheck can be used as a single statement in if/else branches
#define glCheck(expr) do { expr; OpenglDebuger::checkError(__FILE__, __LINE__, #expr); } while (false)

#else

// Else, we don't add any overhead
#define glCheck(expr) /*do { expr; OpenglDebuger::checkError(__FILE__, __LINE__, #expr); } while (false)*/
//#define glCheck(expr) (expr)

#endif

class OpenglDebuger
{
public:
	OpenglDebuger(const char *file);
	~OpenglDebuger();

	static inline void SetIgnore(bool v)
	{
		ignore = v;
	}
	static void checkError(const char *file, int line, const char *expr);

	static inline void PushGroup(GLuint id, GLsizei length, const char* message)
	{
		glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, id, length, message);
	}

	static inline void EndGroup()
	{
		glPopDebugGroup();
	}
private:
	static void APIENTRY callBack(GLenum source​, GLenum type​, GLuint id​,
	GLenum severity​, GLsizei length​, const GLchar* message​, const void* userParam​);

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

	__forceinline CDebugSection(size_t length, const char* message)
	{
		OpenglDebuger::PushGroup(0, static_cast<GLsizei>(length), message);
	}
	__forceinline ~CDebugSection()
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
	inline void object_label(GLenum id, GLuint object, std::string const &label)
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
