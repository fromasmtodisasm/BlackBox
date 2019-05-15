#include <BlackBox/Material.hpp>
#include <BlackBox/Opengl.hpp>
#include <BlackBox/CShader.hpp>
#include <BlackBox/Object.hpp>
#include <BlackBox/CCamera.hpp>

void Material::apply(Object *object, CCamera *camera)
{
  GLenum block = GL_TEXTURE0;
  //program->use();
  program->setUniformValue("Model", object->getTransform());
  program->setUniformValue("View", camera->getViewMatrix());
  program->setUniformValue("Projection", camera->getProjectionMatrix());
  //program->setUniformValue("lightPos", glm::vec3(0,0,0));//m_Objs["light"]->m_transform.position);
  program->setUniformValue("viewPos", camera->Position);
  program->setUniformValue("lightColor", glm::vec3(1,1,1.0));

  if (hasTexture)
  {

    program->setUniformValue("material.shininess", shininess);
    program->setUniformValue("diffuseColor", glm::vec3(1.0));
    if (diffuse != nullptr)
    {
      activeTexture(block, "material.diffuse", diffuse);
      block++;
    }
    if (specular != nullptr)
    {
      activeTexture(block, "material.specular", specular);
      block++;
    }
    if (bump != nullptr)
    {
      activeTexture(block, "material.bump", bump);
      block++;
    }
  }
  else {
    program->setUniformValue("diffuseColor", diffuseColor);
    /*
    program->setUniformValue("material.ambient",  glm::vec3(1.0f, 0.5f, 0.31f));
    program->setUniformValue("material.diffuse",  glm::vec3(1.0f, 0.5f, 0.31f));
    program->setUniformValue("material.specular", glm::vec3(0.5f, 0.5f, 0.5f ));
    program->setUniformValue("material.shininess", 32.0f);
    */
  }
}

void Material::setTexture(Texture *texture, const char *type)
{
  hasTexture = true;
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
      //if (!hasTexture) hasTexture = false;
    }
    }
}

void Material::activeTexture(uint32_t block, const char *uniform, Texture* texture)
{
  glActiveTexture(block);
  glBindTexture(GL_TEXTURE_2D, texture->id);
  glUniform1i(glGetUniformLocation(program->get(), uniform), static_cast<GLint>(block));
}
