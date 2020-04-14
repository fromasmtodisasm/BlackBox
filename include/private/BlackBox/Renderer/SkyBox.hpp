#pragma once
#include <BlackBox/Renderer/IDrawable.hpp>
#include <BlackBox/Renderer/OpenGL/Core.hpp>
#include <BlackBox/Renderer/Texture.hpp>
#include <BlackBox/Renderer/TextureCube.hpp>
#include <BlackBox/Renderer/VertexBuffer.hpp>
#include <BlackBox/Renderer/Shader.hpp>
#include <BlackBox/Resources/MaterialManager.hpp>
#include <BlackBox/Renderer/Pipeline.hpp>
#include <BlackBox/Renderer/VertexFormats.hpp>

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
    ProgramDesc pd = {
      "screen_shader",
      ShaderDesc("skybox.vs"),
      ShaderDesc("skybox.frag")
    };

    MaterialManager::instance()->loadProgram(pd, false);
    shader = MaterialManager::instance()->getProgram(pd.name);

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
    VertexArrayObject::Attributes attributes;
    attributes.stride = 3 * sizeof(float);
    attributes.attributes[VertexArrayObject::POSITION] = 0;
		vertexBuffer = gEnv->pRenderer->CreateBuffer(36, VERTEX_FORMAT_P3F, "SkyBox");
		gEnv->pRenderer->UpdateBuffer(vertexBuffer, skyboxVertices, 36, false);
  }
  // Унаследовано через IDrawable
  virtual void draw(void* data) override
  {
    glm::mat4 View = Pipeline::instance()->view;
    glm::mat4 Projection = Pipeline::instance()->projection;
    glCheck(glDepthMask(GL_FALSE));
    glCheck(glDepthFunc(GL_LEQUAL));
    shader->Use();
    // ... задание видовой и проекционной матриц
    shader->Uniform(glm::mat4(glm::mat3(View)), "View");
    shader->Uniform(Projection, "Projection");
	shader->BindTextureUnit2D(Pipeline::instance()->skyBox->id, 0);

    texture->bind();
		gEnv->pRenderer->DrawBuffer(vertexBuffer, nullptr, 0, 0, GL_TRIANGLES);

    glCheck(glDepthFunc(GL_LESS));
    glCheck(glDepthMask(GL_TRUE));
  }

  void setTextureCube(TextureCube* t)
  {
    texture = t;
  }
};