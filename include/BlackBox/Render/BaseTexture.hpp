#ifndef BASETEXTUREXTURE_HPP
#define BASETEXTUREXTURE_HPP

#include <string>
#include <memory>
#include <BlackBox/Render/IRender.hpp>
#ifdef NVTT
#include <nvtt/nvtt.h>
#endif

class Image;

enum TextureType
{
  DIFFUSE,
  SPECULAR,
  BUMP,
  NORMAL,
  MASK,
  EMISSIVE,
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
    :
    width(0),
    height(0),
    type(TextureType::DIFFUSE),
    id(-1),
    path(std::make_shared<std::string>("")),
    name("")
  {}
#ifdef NVTT
  void GetMipMapLevel(int level, nvtt::Surface& surface);
  void SaveMipMaps();
#endif

  virtual int getWidth() override;
  virtual int getHeight() override;
  virtual int getId() override;
};

#endif // BASETEXTUREXTURE_HPP