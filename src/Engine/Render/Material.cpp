#include <BlackBox/Material.hpp>
#include <BlackBox/Render/Opengl.hpp>
#include <BlackBox/Render/CShader.hpp>
#include <BlackBox/Object.hpp>
#include <BlackBox/CCamera.hpp>
#include <BlackBox/Resources/MaterialManager.hpp>
#include <BlackBox/Render/OpenglDebug.hpp>
#include <BlackBox/Render/Pipeline.hpp>

void Material::apply(Object *object, CCamera *camera)
{
  GLenum block = GL_TEXTURE0;
  //program->use();
  Pipeline::instance()->model = object->getTransform();
  Pipeline::instance()->view = camera->getViewMatrix();
  Pipeline::instance()->projection = camera->getProjectionMatrix();
  Pipeline::instance()->view_pos = camera->Position;

  if (hasTexture)
  {
    if (diffuse[current_diffuse] != nullptr)
    {
      activeTexture(GL_TEXTURE0, "diffuseMap", diffuse[current_diffuse]);
      block++;
    }
    if (specular != nullptr)
    {
      //activeTexture(GL_TEXTURE1, "material.specular", specular);

      block++;
    }
    if (bump != nullptr)
    {
      activeTexture(GL_TEXTURE2, "material.bump", bump);
      block++;
    }
    if (normal != nullptr)
    {
      activeTexture(GL_TEXTURE2, "normalMap", normal);
      block++;
    }
    else
    {
      activeTexture(GL_TEXTURE2, "normalMap", defaultMaterial->normal);
    }
  }
  else {
    program->setUniformValue( diffuseColor,"diffuseColor");
    /*
    program->setUniformValue("material.ambient",  glm::vec3(1.0f, 0.5f, 0.31f));
    program->setUniformValue("material.diffuse",  glm::vec3(1.0f, 0.5f, 0.31f));
    program->setUniformValue("material.specular", glm::vec3(0.5f, 0.5f, 0.5f ));
    program->setUniformValue("material.shininess", 32.0f);
    */
  }
	program->setup();
}

void Material::setTexture(Texture *texture, const char *type)
{
  hasTexture = true;
  texture->setType(type);
  switch(texture->type)
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

void Material::activeTexture(uint32_t block, const char *uniform, BaseTexture* texture)
{
	texture->bind();
  glCheck(glUniform1i(glGetUniformLocation(program->get(), uniform), texture->unit));
}
