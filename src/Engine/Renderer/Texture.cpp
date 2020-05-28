#include <BlackBox/Renderer/Texture.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Renderer/OpenGL/Debug.hpp>
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

void Texture::setType(const char* type)
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

const char* Texture::typeToStr()
{
  std::string result;
  switch (type)
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
  Image img;
  bool hasAlpha = false;
  if (!img.load((texture_root + name).c_str(), &hasAlpha))
    return false;
  auto t = create(img.width, img.height, UNKNOWN, hasAlpha, name, img.data); 
  if (t != nullptr)
  {
	  std::swap(*this, *t); 
  }
  return t == nullptr;
}

Texture* Texture::create(int width, int height, TextureType type, bool hasAlpha, const std::string & name, bool createMipChain, void* data)
{
	Texture* t = new Texture;
  bool status = true;
  GLint internalFormat, inputFormat;
  GLint filterMin, filterMag;
  GLint wrapS, wrapT;
  GLint inputDataType;

  gl::GenTextures(1, &t->id);
  gl::BindTexture2D(t->id);
  switch (type)
  {
  case TextureType::DEPTH:
    internalFormat = inputFormat = GL_DEPTH_COMPONENT;
    filterMin = filterMag = GL_LINEAR;
    wrapS = wrapT = GL_CLAMP_TO_BORDER;
    inputDataType = GL_FLOAT;
    //attachment =
    break;
  case TextureType::LDR_RENDER_TARGET:
    internalFormat = inputFormat = GL_RGBA;
    filterMin = filterMag = GL_LINEAR;
    wrapS = wrapT = GL_CLAMP_TO_EDGE;
    inputDataType = GL_UNSIGNED_BYTE;
    break;
  case TextureType::HDR_RENDER_TARGET:
    internalFormat = GL_RGBA16F;
    inputFormat = GL_RGBA;
    filterMin = filterMag = GL_LINEAR;
    wrapS = wrapT = GL_CLAMP_TO_EDGE;
    inputDataType = GL_FLOAT;
    break;
  default:
    internalFormat = inputFormat = GL_RGB;
    filterMin = filterMag = GL_LINEAR;
    wrapS = wrapT = GL_CLAMP_TO_EDGE;
    inputDataType = GL_UNSIGNED_BYTE;
		gl::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		gl::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    break;

		if (hasAlpha)
		{
			inputFormat = GL_RGBA;
			internalFormat = GL_SRGB_ALPHA;
		}
  }

  gl::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMin);
  gl::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMag);

  if (type == DEPTH)
  {
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
  }
  glTexImage2D(
    GL_TEXTURE_2D, 0, internalFormat,
    width, height,
    0,
    inputFormat, inputDataType, data
  );

  if (createMipChain)
  {
		glCheck(glGenerateMipmap(GL_TEXTURE_2D));
  }

  gl::BindTexture2D(0);
  debuger::texture_label(t->id, name);
	return t;
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