#include <BlackBox/Renderer/AuxRenderer.hpp>
#include <BlackBox/Renderer/Camera.hpp>
#include <BlackBox/Renderer/OpenGL/Core.hpp>
#include <BlackBox/Renderer/Pipeline.hpp>
#include <BlackBox/Renderer/VertexFormats.hpp>

using P3F = struct_VERTEX_FORMAT_P3F;
using VecPos = std::vector<P3F>;

CRenderAuxGeom::CRenderAuxGeom()
{
  // Cube 1x1x1, centered on origin
   static P3F vertices[] = {
    Vec3{-0.5f, -0.5f, -0.5f},
    Vec3{ 0.5, -0.5, -0.5},
    Vec3{ 0.5,  0.5, -0.5},
    Vec3{-0.5,  0.5, -0.5},
    Vec3{-0.5, -0.5,  0.5},
    Vec3{ 0.5, -0.5,  0.5},
    Vec3{ 0.5,  0.5,  0.5},
    Vec3{-0.5,  0.5,  0.5}
  };
  static GLushort elements[] = {
    0, 1, 2, 3,
    4, 5, 6, 7,
    0, 4, 1, 5, 
    2, 6, 3, 7
  };
   auto cnt = sizeof vertices / sizeof P3F;
  m_BoundingBox = gEnv->pRenderer->CreateBuffer(cnt, VERTEX_FORMAT_P3F, "BoundingBox", false);
  gEnv->pRenderer->UpdateBuffer(m_BoundingBox, vertices, cnt, false);

  m_BB_IndexBuffer = new SVertexStream;
  gEnv->pRenderer->CreateIndexBuffer(m_BB_IndexBuffer, elements, (sizeof elements / sizeof GLushort));
}

//TODO: Довести до ума, нужно учитывать трансформации объекта
void CRenderAuxGeom::DrawAABB(Vec3 min, Vec3 max)
{
  auto shader = Pipeline::bindProgram("bb");

  glm::vec3 size = glm::vec3(max.x - min.x, max.y - min.y, max.z - min.z);
  glm::vec3 center = glm::vec3((min.x + max.x) / 2, (min.y + max.y) / 2, (min.z + max.z) / 2);
  glm::mat4 transform = glm::translate(glm::mat4(1), center) * glm::scale(glm::mat4(1), size);

  const auto& cam = gEnv->pRenderer->GetCamera();
  shader->Uniform(transform, "model");
  shader->Uniform(cam.getViewMatrix(), "view");
  shader->Uniform(cam.getProjectionMatrix(), "projection");


  gEnv->pRenderer->DrawBuffer(m_BoundingBox, m_BB_IndexBuffer, 4, 4, static_cast<int>(RenderPrimitive::LINE_LOOP));
  gEnv->pRenderer->DrawBuffer(m_BoundingBox, m_BB_IndexBuffer, 4, 8, static_cast<int>(RenderPrimitive::LINE_LOOP));
  gEnv->pRenderer->DrawBuffer(m_BoundingBox, m_BB_IndexBuffer, 8, 8, static_cast<int>(RenderPrimitive::LINES));
}
