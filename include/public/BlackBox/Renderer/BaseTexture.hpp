#ifndef BASETEXTUREXTURE_HPP
#define BASETEXTUREXTURE_HPP

#include <string>
#include <memory>
#include <BlackBox/Renderer/IRender.hpp>
#ifdef NVTT
#include <nvtt/nvtt.h>
#endif

class Image
{
public:
  int width;
  int height;
  int bpp;
  uint8_t* data;

  Image() : width(0), height(0), bpp(0), data(nullptr) {}
  ~Image();
  bool load(const char* name, bool* hasAlpha);
  void free();
};

enum TextureType
{
  DIFFUSE,
  SPECULAR,
  BUMP,
  NORMAL,
  MASK,
  EMISSIVE,

  HDR_RENDER_TARGET,
  LDR_RENDER_TARGET,
	DEPTH,
  UNKNOWN
};

class BaseTexture : public ITexture
{
  public:
	int width;
	int height;
	TextureType type;
	uint id;
	int unit = 0;
	std::shared_ptr<std::string> path;
	std::string name;
	std::string texture_root = "res/images/";

	BaseTexture()
		: width(0),
		  height(0),
		  type(TextureType::DIFFUSE),
		  id(-1),
		  path(std::make_shared<std::string>("")),
		  name("")
	{
	}
	BaseTexture(int width, int height, TextureType type, const std::string& name)
		: width(width),
		  height(height),
		  type(type),
		  name(name)
	{
	
	}
#ifdef NVTT
  void GetMipMapLevel(int level, nvtt::Surface& surface);
  void SaveMipMaps();
#endif

  virtual int getWidth() const override;
  virtual int getHeight() const override;
  virtual int getId() const override;
	virtual uint getUnit() const override
	{
		return unit;
	}
};

#endif // BASETEXTUREXTURE_HPP