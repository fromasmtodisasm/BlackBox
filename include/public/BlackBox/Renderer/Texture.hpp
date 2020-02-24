#ifndef TEXTUREXTURE_HPP
#define TEXTUREXTURE_HPP

#include <string>
#include <memory>
#include <BlackBox/Renderer/BaseTexture.hpp>
#ifdef NVTT
#include <nvtt/nvtt.h>
#endif

class Texture : public BaseTexture
{
public:
  Texture();
  Texture(uint id);
  Texture(std::string name);
#ifdef NVTT
  void GetMipMapLevel(int level, nvtt::Surface& surface);
  void SaveMipMaps();
#endif
  virtual void setType(const char* TextureType) override;
  virtual const char* typeToStr() override;

  // ������������ ����� BaseTexture
  virtual bool load(const char* name) override;

  // Inherited via BaseTexture
  virtual void bind() override;
  virtual void setUnit(uint unit) override;
};

#endif // TEXTUREXTURE_HPP