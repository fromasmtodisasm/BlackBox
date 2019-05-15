#include <BlackBox/Material.hpp>
#include <BlackBox/Opengl.hpp>
#include <BlackBox/CShader.hpp>
#include <BlackBox/Object.hpp>
#include <BlackBox/CCamera.hpp>

void Material::apply(Object *object, CCamera *camera)
{
  GLenum block = GL_TEXTURE0;
  //program->use();
  program->setUniformValue( object->getTransform(),"Model");
  program->setUniformValue( camera->getViewMatrix(),"View");
  program->setUniformValue( camera->getProjectionMatrix(),"Projection");
  program->setUniformValue( camera->Position,"viewPos");

  if (hasTexture)
  {

    program->setUniformValue( 128.0f,"material.shininess");
    if (diffuse != nullptr)
    {
      activeTexture(GL_TEXTURE0, "material.diffuse", diffuse);
      block++;
    }
    if (specular != nullptr)
    {
      activeTexture(GL_TEXTURE1, "material.specular", specular);
      block++;
    }
    if (bump != nullptr)
    {
      activeTexture(GL_TEXTURE2, "material.bump", bump);
      block++;
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
  int test;
  glActiveTexture(block);
  glBindTexture(GL_TEXTURE_2D, texture->id);
  glUniform1i(test = glGetUniformLocation(program->get(), uniform), static_cast<GLint>(block));
}
