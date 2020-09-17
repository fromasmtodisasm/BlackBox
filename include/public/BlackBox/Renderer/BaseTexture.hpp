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
  int channels;
  std::vector<uint8_t> data;
  bool compressed = false;
  int format;
  uint blockSize = 0;
  uint mipMapCount = 0;


  Image() : width(0), height(0), bpp(0)  {}
  Image(int w, int h, int bpp, std::vector<uint8_t>&& data, bool compressed = false, int format = 0, uint bs = 0, uint mMC = 0)
	  : width(w), height(h), bpp(bpp), data(data), compressed(compressed), format(format), blockSize(bs), mipMapCount(mMC)
  {
  }
  ~Image();
  bool load(const char* name, bool* hasAlpha);
private:
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