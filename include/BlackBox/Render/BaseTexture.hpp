#ifndef BASETEXTUREXTURE_HPP
#define BASETEXTUREXTURE_HPP

#include <string>
#include <memory>
#include <BlackBox/Render/Opengl.hpp>
#include <BlackBox/ITexture.hpp>
#include <SFML/Graphics.hpp>
#ifdef NVTT
#include <nvtt/nvtt.h>
#endif

enum TextureType
{
    DIFFUSE,
    SPECULAR,
    BUMP,
    NORMAL,
    MASK,
    UNKNOWN
};

class BaseTexture : public ITexture
{
public:
  int width;
  int height;
  TextureType type;
  GLuint id;
  std::shared_ptr<std::string> path;
  std::string name;

  BaseTexture()
		:
		width(0),
		height(0),
		type(TextureType::DIFFUSE),
		id(-1),
		path(std::make_shared<std::string>("")),
		name("")
	{}
#ifdef NVTT
	void GetMipMapLevel(int level, nvtt::Surface &surface);
	void SaveMipMaps();
#endif
};

#endif // BASETEXTUREXTURE_HPP
