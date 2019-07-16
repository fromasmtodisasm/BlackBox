#include <BlackBox/Texture.hpp>
#include <BlackBox/Utils/AlphaDistribution.h>
#include <iostream>
#include <nvtt/nvtt.h>
#include <ctime>
#include <memory>

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
	/*
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
	*/

	//if (name == "alpha_test/low_albedo.tga")
	//{
	if (name == "check.png")
	{
		cout << "" << endl;
	}
		::srand(time(0));
		nvtt::Surface surface;
		//surface.setAlphaMode(nvtt::AlphaMode::AlphaMode_Transparency);
		if (!surface.load(path.c_str(), &has_alpha))
		{
			// TODO: LOG IT
			return;
		}
		w = surface.width();
		h = surface.height();
		surface.flipY();
		auto pix = std::make_unique<float[]>(w * h * sizeof(float));
		const float* r, * g, * b, * a;
		r = surface.channel(0);
		g = surface.channel(1);
		b = surface.channel(2);

		int step = 3;
		int format = surface.alphaMode();
		if (surface.channel(3) != nullptr)
		{
			has_alpha = true;
			a = surface.channel(3);
			step = 4;
			inputFormat = GL_RGBA;
			internalFormat = GL_RGBA;
		}

		for (int i = 0; i < w * h; i++)
		{
			pix[i*step]		 = r[i];
			pix[i*step + 1] = g[i];
			pix[i*step + 2] = b[i];
			if (has_alpha)
				pix[i*step + 3] = a[i];
		}
		pixels = pix.get();
		//pixels = surface.data();
		//inputFormat = GL_BGRA;
		inputDataType = GL_FLOAT;
	//}
  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_2D, id);

  glTexImage2D(
    GL_TEXTURE_2D, 0, internalFormat,
    w, h,
    0,
    inputFormat, inputDataType, pixels 
  );

  glGenerateMipmap(GL_TEXTURE_2D);
  if (true)
  {
    cy::AlphaDistribution::FixTextureAlpha(cy::AlphaDistribution::Method::METHOD_PYRAMID, id);
  }
	if (name == "low2_a.dds")
		SaveMipMaps();
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


void Texture::GetMipMapLevel(int level, nvtt::Surface &surface)
{
	int width = 0, height = 0;
	glBindTexture(GL_TEXTURE_2D, id);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, level, GL_TEXTURE_WIDTH, &width);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, level, GL_TEXTURE_HEIGHT, &height);
	if (width * height == 0) return;

	std::vector<unsigned char> image(width * height * 4);
	glGetTexImage(GL_TEXTURE_2D, level, GL_BGRA, GL_UNSIGNED_BYTE, image.data());
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


	glGetTexLevelParameteriv(GL_TEXTURE_2D, level, GL_TEXTURE_WIDTH, &width);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, level, GL_TEXTURE_HEIGHT, &height);

	while (width >= 1 && height >= 1) {
		GetMipMapLevel(level, surface);
		surface.flipY();
		compressor.compress(surface, 0, level, compressorOptions, outputOptions);
		level++;
		glGetTexLevelParameteriv(GL_TEXTURE_2D, level, GL_TEXTURE_WIDTH, &width);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, level, GL_TEXTURE_HEIGHT, &height);
	}
}
