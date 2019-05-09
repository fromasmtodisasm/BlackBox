#include <BlackBox/Material.hpp>
#include <BlackBox/Opengl.hpp>
#include <BlackBox/CShader.hpp>
#include <BlackBox/Object.hpp>
#include <BlackBox/CCamera.hpp>

void Material::apply(Object *object, CCamera *camera)
{
  GLenum block = GL_TEXTURE0;
  program->use();
  program->setUniformValue("Model", object->getTransform());
  program->setUniformValue("View", camera->getViewMatrix());
  program->setUniformValue("Projection", camera->getProjectionMatrix());
  program->setUniformValue("lightPos", glm::vec3(4,4,4));//m_Objs["light"]->m_transform.position);
  program->setUniformValue("lightColor", glm::vec3(1,1,1.0));

  if (diffuse != nullptr)
  {
    activeTexture(block, "diffuseTexture", diffuse);
    block++;
  }
  if (specular != nullptr)
  {
    activeTexture(block, "specularTexture", specular);
    block++;
  }
  if (bump != nullptr)
  {
    activeTexture(block, "bumpTexture", bump);
    block++;
  }
}

void Material::setTexture(Texture *texture, const char *type)
{
  texture->setType(type);
  switch(texture->type)
    {
    case TextureType::DIFFUSE:
      this->diffuse = texture;
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
    }
    }
}

void Material::activeTexture(uint32_t block, const char *uniform, Texture* texture)
{
  glActiveTexture(block);
  glBindTexture(GL_TEXTURE_2D, texture->id);
  glUniform1i(glGetUniformLocation(program->get(), uniform), static_cast<GLint>(block));
}
