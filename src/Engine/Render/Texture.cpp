#include <BlackBox/Render/Texture.hpp>
#include <BlackBox/Render/OpenGL/Debug.hpp>
#ifdef ALPHA_DIST
#include <BlackBox/Utils/AlphaDistribution.h>
#endif
#include <iostream>
#include <ctime>
#include <memory>

using namespace std;

Texture::Texture()
{
	id = 0;
}

Texture::Texture(GLuint id) 
{
  BaseTexture::id = id;
  glCheck(glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width));
  glCheck(glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &height));
}

Texture::Texture(std::string name) 
{
	this->name = name;
}

void Texture::setType(const char *type)
{
  std::string t(type);

  if (t == "diffuse")
    this->type = DIFFUSE;
  else if (t == "specular")
    this->type = SPECULAR;
  else if (t == "bump")
    this->type = BUMP;
  else if (t == "normal")
    this->type = NORMAL;
  else if (t == "mask")
    this->type = MASK;
  else if (t == "emissive")
    this->type = EMISSIVE;
  else
    this->type = UNKNOWN;
}

std::string Texture::typeToStr()
{
  std::string result;
  switch(type)
    {
    case TextureType::DIFFUSE:
      return "diffuse";
    case TextureType::SPECULAR:
      return "specular";
    case TextureType::BUMP:
      return "bump";
    case TextureType::NORMAL:
      return "normal";
    case TextureType::MASK:
      return "mask";
    default:
    {
      cout << "Error: unknown texture type" << endl;
      return "";
    }
    }
}

bool Texture::load(const char* name)
{
	
	GLenum inputFormat = GL_RGB;
	GLenum internalFormat = GL_SRGB;
	GLenum inputDataType = GL_UNSIGNED_BYTE;
	bool hasAlpha = false;

	Image img;
	if (!img.load((texture_root + name).c_str(), &hasAlpha))
		return false;
	if (hasAlpha)
	{
		inputFormat = GL_RGBA;
		internalFormat = GL_SRGB_ALPHA;
	}
  glCheck(glGenTextures(1, &id));
  gl::BindTexture2D(id);
	width = img.width;
	height = img.height;
	gl::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	gl::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	gl::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	gl::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	gl::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	gl::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexImage2D(
    GL_TEXTURE_2D, 0, internalFormat,
    img.width, img.height,
    0,
    inputFormat, inputDataType, img.data 
  );

  glCheck(glGenerateMipmap(GL_TEXTURE_2D));
#ifdef ALPHA_DIST
  if (true)
  {
		cy::AlphaDistribution::FixTextureAlpha(cy::AlphaDistribution::Method::METHOD_PYRAMID, id);
  }
#endif
  gl::BindTexture2D(0);
	debuger::texture_label(id, name);
  return true;
}

void Texture::bind()
{
	gl::ActiveTexture(GL_TEXTURE0 + unit);
  gl::BindTexture2D(id);
}

void Texture::setUnit(GLuint unit)
{
	this->unit = unit;
}
