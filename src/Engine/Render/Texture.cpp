#include <BlackBox/Render/Texture.hpp>
#include <BlackBox/Render/OpenglDebug.hpp>
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
  glCheck(glBindTexture(GL_TEXTURE_2D, id));
	width = img.width;
	height = img.height;
	glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
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
  glCheck(glBindTexture(GL_TEXTURE_2D, 0));
	debuger::texture_label(id, name);
  return true;
}

void Texture::bind()
{
	glCheck(glActiveTexture(GL_TEXTURE0 + unit));
	glCheck(glBindTexture(GL_TEXTURE_2D, id));
}

void Texture::setUnit(GLuint unit)
{
	this->unit = unit;
}

#ifdef NVTT

void Texture::GetMipMapLevel(int level, nvtt::Surface &surface)
{
	int width = 0, height = 0;
	glCheck(glBindTexture(GL_TEXTURE_2D, id));
	glCheck(glGetTexLevelParameteriv(GL_TEXTURE_2D, level, GL_TEXTURE_WIDTH, &width));
	glCheck(glGetTexLevelParameteriv(GL_TEXTURE_2D, level, GL_TEXTURE_HEIGHT, &height));
	if (width * height == 0) return;

	std::vector<unsigned char> image(width * height * 4);
	glCheck(glGetTexImage(GL_TEXTURE_2D, level, GL_BGRA, GL_UNSIGNED_BYTE, image.data()));
	std::vector<float> image_f(width * height * 4);
	/*
	for (int i = 0; i < image.size(); i++)
	{
		image_f[i] = (float)image[i] / 255.0f;
	}
	*/
	surface.setImage(nvtt::InputFormat::InputFormat_BGRA_8UB, width, height, 1, image.data());
	//FixAlphaRGBA(method, image.data(), width, height, spp);
	//glTexImage2D(GL_TEXTURE_2D, level, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
}

//! Fixes the alpha values of all mipmap levels of an OpenGL texture, starting with the given level.
//! If the texture does not contain semi-transparent regions, modifying the first level (level zero)
//! is not advisable, since the original values might work better with magnification filtering.
//! If the image will be used with alpha-to-coverage, the spp parameter
//! should indicate the number of alpha samples; otherwise, it should be 1.
void Texture::SaveMipMaps()
{
	int level = 0;
	int width, height;
	nvtt::Surface surface;
	nvtt::OutputOptions outputOptions;
	outputOptions.setFileName("nativeMips.dds");

	nvtt::CompressionOptions compressorOptions;
	compressorOptions.setFormat(nvtt::Format::Format_RGBA);
	nvtt::Compressor compressor;
	GetMipMapLevel(0, surface);
	compressor.outputHeader(surface, surface.countMipmaps(), compressorOptions, outputOptions);


	glCheck(glGetTexLevelParameteriv(GL_TEXTURE_2D, level, GL_TEXTURE_WIDTH, &width));
	glCheck(glGetTexLevelParameteriv(GL_TEXTURE_2D, level, GL_TEXTURE_HEIGHT, &height));

	while (width >= 1 && height >= 1) {
		GetMipMapLevel(level, surface);
		surface.flipY();
		compressor.compress(surface, 0, level, compressorOptions, outputOptions);
		level++;
		glCheck(glGetTexLevelParameteriv(GL_TEXTURE_2D, level, GL_TEXTURE_WIDTH, &width));
		glCheck(glGetTexLevelParameteriv(GL_TEXTURE_2D, level, GL_TEXTURE_HEIGHT, &height));
	}
}

#endif
