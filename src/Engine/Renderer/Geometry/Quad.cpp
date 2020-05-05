#include <BlackBox/Renderer/Quad.hpp>
#include <BlackBox/Renderer/IGeometry.hpp>
#include <BlackBox/Renderer/VertexFormats.hpp>

Quad::Quad()
{
  float verts[] = {
    // positions			  // texCoords
		-1.0f,  1.0f, 0.5f,  0.0f, 1.0f,
		-1.0f, -1.0f, 0.5f,  0.0f, 0.0f,
		 1.0f, -1.0f, 0.5f,  1.0f, 0.0f,

		-1.0f,  1.0f, 0.5f,  0.0f, 1.0f,
		 1.0f, -1.0f, 0.5f,  1.0f, 0.0f,
		 1.0f,  1.0f, 0.5f,  1.0f, 1.0f
  };
	;
	m_VertexBuffer = gEnv->pRenderer->CreateBuffer(6, VertFormatForComponents(false, false, false, true), "screen_quad", false);
	gEnv->pRenderer->UpdateBuffer(m_VertexBuffer, verts, 6, false);
}

Quad::~Quad()
{
	gEnv->pRenderer->ReleaseBuffer(m_VertexBuffer);
}

void Quad::draw() {
	gEnv->pRenderer->DrawBuffer(m_VertexBuffer, nullptr, 0, 0, static_cast<int>(RenderPrimitive::TRIANGLES));
}

bool Quad::init()
{
  return true;
}