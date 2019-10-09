// drawer2D.h

#ifndef __DRAWER2D_H__
#define __DRAWER2D_H__

#include <BlackBox/Render/Opengl.hpp>
#include <BlackBox/Profiler/Utils.h>

// Simple class to draw strings in a [-1;1]x[-1;1] coordinates system
class Drawer2D
{
private:
	GLuint m_id_vao_rect;
	GLuint m_id_vao_string;

	GLuint	m_id_vbo_rect;
	GLuint m_id_vbo_string;

	// Unicolor shader
	GLuint	m_id_vert_color;
	GLuint	m_id_frag_color;
	GLuint	m_id_prog_color;

	GLint	m_id_uniform_color;

	// Font
	GLuint	m_id_tex_font;
	GLint	m_id_uniform_tex_font;
	GLint	m_id_uniform_font_color;

	GLuint	m_id_vert_font;
	GLuint	m_id_frag_font;
	GLuint	m_id_prog_font;

	// Window size
	int		m_win_w;
	int		m_win_h;

public:
	Drawer2D() {}

	bool	init(int win_w, int win_h);
	void	shut();

	void	onResize(int win_w, int win_h)	{m_win_w = win_w;	m_win_h = win_h;	}

	void	drawRect(const Utils::Rect& rect, const Utils::Color& color=Utils::COLOR_WHITE, float alpha=1.0f);
	void	drawString(const char* str, float x, float y, const Utils::Color& color=Utils::COLOR_WHITE);

private:
	bool	initFont();
};

extern Drawer2D drawer2D;

#endif // __DRAWER2D_H__
