#include <BlackBox/Texture.hpp>
#include <BlackBox/Utils/AlphaDistribution.h>
#include <iostream>
#ifdef NVTT
#include <nvtt/nvtt.h>
#endif
#include <ctime>

using namespace std;

Texture::Texture()
{

}

Texture::Texture(std::string name, bool alphaDistMips)
{
	std::string path = "res/images/" + name;
	int w = 0;
	int h = 0;
	const void* pixels = 0;
	bool has_alpha = false;
	GLenum inputFormat = GL_RGB;
	GLenum internalFormat = GL_RGB;
	GLenum inputDataType = GL_UNSIGNED_BYTE;

  sf::Texture text;
  text.loadFromFile(path);
  sf::Image img_data;

#ifndef NVTT
  if (!img_data.loadFromFile(path))
  {
		cout << "rwer" << endl;
  }
	else
	{
		w = img_data.getSize().x;
		h = img_data.getSize().y;
		pixels = img_data.getPixelsPtr();
	}
#else
	::srand(time(0));
	nvtt::Surface surface;
	if (!surface.load(path.c_str()))
	{
		// TODO: LOG IT
		return;
	}
	w = surface.width();
	h = surface.height();
	surface.flipY();
	float* pix = new float[w * h * sizeof(float)];// surface.data();
	const float* r, * g, * b, * a;
	r = surface.channel(0);
	g = surface.channel(1);
	b = surface.channel(2);

	int step = 3;
	if (has_alpha)
	{
		a = surface.channel(3);
		step = 4;
		inputFormat = GL_RGBA;
		internalFormat = GL_RGBA;
	}

	for (int i = 0; i < w * h; i+=step)
	{
		pix[i]		 = r[i];
		pix[i + 1] = g[i];
		pix[i + 2] = b[i];
		if (has_alpha)
			pix[i + 3] = a[i]; 
	}
	pixels = pix;
	//inputFormat = GL_BGRA;
	inputDataType = GL_FLOAT;
#endif // !NVTT

  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_2D, id);

  glTexImage2D(
    GL_TEXTURE_2D, 0, internalFormat,
    w, h,
    0,
    inputFormat, inputDataType, pixels 
  );

  glGenerateMipmap(GL_TEXTURE_2D);
  if (alphaDistMips)
  {
    cy::AlphaDistribution::FixTextureAlpha(cy::AlphaDistribution::Method::METHOD_PYRAMID, id);
  }
  glBindTexture(GL_TEXTURE_2D, 0);
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
