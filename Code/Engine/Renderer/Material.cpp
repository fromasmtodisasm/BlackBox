#if 0
#include <BlackBox/Renderer/Material.hpp>
#include <BlackBox/Renderer/Camera.hpp>
#include <BlackBox/Renderer/Object.hpp>
#include <BlackBox/Renderer/OpenGL/Core.hpp>
#include <BlackBox/Renderer/OpenGL/Debug.hpp>
#include <BlackBox/Renderer/Pipeline.hpp>
#include <BlackBox/Renderer/Shader.hpp>
#include <BlackBox/Renderer/MaterialManager.hpp>

void Material::apply(CStatObj* object)
{
  GLenum block = GL_TEXTURE0;

  if (hasTexture)
  {
    if (diffuse[current_diffuse] != nullptr)
    {
      activeTexture("diffuseMap", diffuse[current_diffuse]);
      block++;
    }
    if (specular != nullptr)
    {
      activeTexture("specularMap", specular);
      program->Uniform(true, "has_specular");
      block++;
    }
    else
    {
      program->Uniform(false, "has_specular");
    }
    if (bump != nullptr)
    {
      activeTexture("material.bump", bump);
      block++;
    }
    if (normal != nullptr)
    {
      activeTexture("normalMap", normal);
      block++;
    }
    else
    {
      activeTexture("normalMap", defaultMaterial->normal);
    }
    if (emissive != nullptr)
    {
      activeTexture("emissiveMap", emissive);
      program->Uniform(true, "has_emissive");
      program->Uniform(GetISystem()->GetIConsole()->GetCVar("ef")->GetFVal(), "emissive_factor");
    }
    else
    {
      program->Uniform(false, "has_emissive");
    }
  }
  else {
    program->Uniform(diffuseColor, "diffuseColor");
  }
  program->Uniform(object->uvMatrix, "uvMatrix");
  //program->setup();
}

void Material::setTexture(Texture* texture, const char* type)
{
  hasTexture = true;
  texture->setType(type);
  switch (texture->type)
  {
  case TextureType::DIFFUSE:
    this->diffuse.push_back(texture);
    break;
  case TextureType::SPECULAR:
    this->specular = texture;
    break;
  case TextureType::BUMP:
    this->bump = texture;
    break;
  case TextureType::NORMAL:
    this->normal = texture;
    break;
  case TextureType::MASK:
    this->mask = texture;
    break;
  default:
  {
    // TODO: LOG IT
    //if (!hasTexture) hasTexture = false;
  }
  }
}

void Material::nextDiffuse()
{
  current_diffuse = (current_diffuse + 1) % diffuse.size();
}

void Material::prevDiffuse()
{
  current_diffuse = (current_diffuse - 1) % diffuse.size();
}

void Material::activeTexture(const char* uniform, BaseTexture* texture)
{
  gl::ActiveTexture(GL_TEXTURE0 + texture->unit);
  texture->bind();
  //glCheck(glUniform1i(glGetUniformLocation(program->get(), uniform), texture->unit));
  program->Uniform(texture->unit, uniform);
}
#endif