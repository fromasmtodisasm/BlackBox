#ifndef TEXTUREXTURE_HPP
#define TEXTUREXTURE_HPP

#include <string>
#include <memory>
#include <string_view>
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

  // Унаследовано через BaseTexture
  virtual bool load(const char* name) override;
  virtual bool load(std::string_view name);

  // Inherited via BaseTexture
  virtual void bind() override;
  virtual void setUnit(uint unit) override;
  static Texture* create(const Image& img, TextureType type, const std::string& name, bool createMipChain = false, bool is_msaa = false);

  bool isMS = false;
};

#endif // TEXTUREXTURE_HPP
