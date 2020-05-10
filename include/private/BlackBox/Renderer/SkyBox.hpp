#pragma once
#include <BlackBox/Renderer/IDrawable.hpp>
#include <BlackBox/Renderer/Texture.hpp>
#include <BlackBox/Renderer/TextureCube.hpp>
#include <BlackBox/Renderer/Shader.hpp>
#include <BlackBox/Renderer/MaterialManager.hpp>
#include <BlackBox/Renderer/VertexFormats.hpp>
#include <BlackBox/Renderer/Camera.hpp>

class SkyBox : public IDrawable
{
public:
  _smart_ptr<TextureCube> texture;
	CVertexBuffer* vertexBuffer;
  BaseShaderProgramRef shader;

  SkyBox(TextureCube* t)
    :
    texture(t)
  {

    shader = gEnv->pRenderer->Sh_Load("SkyBox", 0);

    shader->Use();
    shader->Uniform(0, "skybox");
    shader->Unuse();

    static float skyboxVertices[] = {
      // positions
      -1.0f,  1.0f, -1.0f,
      -1.0f, -1.0f, -1.0f,
       1.0f, -1.0f, -1.0f,
       1.0f, -1.0f, -1.0f,
       1.0f,  1.0f, -1.0f,
      -1.0f,  1.0f, -1.0f,

      -1.0f, -1.0f,  1.0f,
      -1.0f, -1.0f, -1.0f,
      -1.0f,  1.0f, -1.0f,
      -1.0f,  1.0f, -1.0f,
      -1.0f,  1.0f,  1.0f,
      -1.0f, -1.0f,  1.0f,

       1.0f, -1.0f, -1.0f,
       1.0f, -1.0f,  1.0f,
       1.0f,  1.0f,  1.0f,
       1.0f,  1.0f,  1.0f,
       1.0f,  1.0f, -1.0f,
       1.0f, -1.0f, -1.0f,

      -1.0f, -1.0f,  1.0f,
      -1.0f,  1.0f,  1.0f,
       1.0f,  1.0f,  1.0f,
       1.0f,  1.0f,  1.0f,
       1.0f, -1.0f,  1.0f,
      -1.0f, -1.0f,  1.0f,

      -1.0f,  1.0f, -1.0f,
       1.0f,  1.0f, -1.0f,
       1.0f,  1.0f,  1.0f,
       1.0f,  1.0f,  1.0f,
      -1.0f,  1.0f,  1.0f,
      -1.0f,  1.0f, -1.0f,

      -1.0f, -1.0f, -1.0f,
      -1.0f, -1.0f,  1.0f,
       1.0f, -1.0f, -1.0f,
       1.0f, -1.0f, -1.0f,
      -1.0f, -1.0f,  1.0f,
       1.0f, -1.0f,  1.0f
    };
		vertexBuffer = gEnv->pRenderer->CreateBuffer(36, VERTEX_FORMAT_P3F, "SkyBox");
		gEnv->pRenderer->UpdateBuffer(vertexBuffer, skyboxVertices, 36, false);
  }
  // Унаследовано через IDrawable
  virtual void draw(SRenderParams& renderParams) override
  {
    //glCheck(glDepthMask(GL_FALSE));
    //glCheck(glDepthFunc(GL_LEQUAL));
    shader->Use();
    // ... задание видовой и проекционной матриц
    shader->Uniform(gEnv->pRenderer->GetCamera().getViewMatrix(), "View");
    shader->Uniform(gEnv->pRenderer->GetCamera().getProjectionMatrix(), "Projection");
		shader->BindTextureUnit2D(texture->getId(), 0);

    texture->bind();
		gEnv->pRenderer->DrawBuffer(vertexBuffer, nullptr, 0, 0, static_cast<int>(RenderPrimitive::TRIANGLES));

    //glCheck(glDepthFunc(GL_LESS));
    //glCheck(glDepthMask(GL_TRUE));
  }

  void setTextureCube(TextureCube* t)
  {
    texture = t;
  }
};