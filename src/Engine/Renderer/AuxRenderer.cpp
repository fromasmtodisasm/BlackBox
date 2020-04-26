#include <BlackBox/Renderer/AuxRenderer.hpp>
#include <BlackBox/Renderer/Camera.hpp>
#include <BlackBox/Renderer/VertexFormats.hpp>
#include <BlackBox/Renderer/Pipeline.hpp>

using P3F = SVF_P3F;
using VecPos = std::vector<P3F>;

namespace
{

}

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
  static uint16 elements[] = {
#if 0
    2, 1, 0,
    3, 2, 0,

    6, 5, 4,
    7, 6, 4,

    1, 0, 4,
    5, 4, 1,

    //0, 4, 1, 5, 
    2, 6, 3, 7
#else
0,3,2,1,
2,3,7,6,
0,4,7,3,
1,2,6,5,
4,5,6,7,
0,1,5,4
#endif
  };
  auto cnt = sizeof vertices / sizeof P3F;
  m_BoundingBox = gEnv->pRenderer->CreateBuffer(cnt, VERTEX_FORMAT_P3F, "BoundingBox", false);
  gEnv->pRenderer->UpdateBuffer(m_BoundingBox, vertices, cnt, false);

  m_BB_IndexBuffer = new SVertexStream;
  gEnv->pRenderer->CreateIndexBuffer(m_BB_IndexBuffer, elements, (sizeof elements / sizeof uint16));
}

CRenderAuxGeom::~CRenderAuxGeom()
{
  SAFE_DELETE(m_BoundingBox);
  SAFE_DELETE(m_BB_IndexBuffer);
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
  shader->Uniform(0.1f, "alpha");


  {
    RSS(gEnv->pRenderer, BLEND, true);
    RSS(gEnv->pRenderer, CULL_FACE, false);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    gEnv->pRenderer->DrawBuffer(m_BoundingBox, m_BB_IndexBuffer, 24, 0, static_cast<int>(RenderPrimitive::TRIANGLES));
  }
  //gEnv->pRenderer->DrawBuffer(m_BoundingBox, m_BB_IndexBuffer, 4, 18, static_cast<int>(RenderPrimitive::LINES));
}
