#include <BlackBox/Material.hpp>
#include <BlackBox/Render/Opengl.hpp>
#include <BlackBox/Render/Shader.hpp>
#include <BlackBox/Object.hpp>
#include <BlackBox/Camera.hpp>
#include <BlackBox/Resources/MaterialManager.hpp>
#include <BlackBox/Render/OpenglDebug.hpp>
#include <BlackBox/Render/Pipeline.hpp>

void Material::apply(Object *object)
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
			program->setUniformValue(true, "has_specular");
      block++;
    }
		else
		{
			program->setUniformValue(false, "has_specular");
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
			program->setUniformValue(true, "has_emissive");
			program->setUniformValue(GetISystem()->GetIConsole()->GetCVar("ef")->GetFVal(), "emissive_factor");
		}
		else
		{
			program->setUniformValue(false, "has_emissive");
		}
  }
  else {
    program->setUniformValue( diffuseColor,"diffuseColor");
  }
	program->setUniformValue(object->uvMatrix, "uvMatrix");
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

void Material::activeTexture(const char *uniform, BaseTexture* texture)
{
	glCheck(glActiveTexture(GL_TEXTURE0 + texture->unit));
	texture->bind();
  //glCheck(glUniform1i(glGetUniformLocation(program->get(), uniform), texture->unit));
	program->setUniformValue(texture->unit, uniform);
}
