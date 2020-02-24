#pragma once
#include <string>
#include <memory>
//#include <BlackBox/Renderer/OpenGL/Core.hpp>
#include <BlackBox/Renderer/BaseTexture.hpp>
#ifdef NVTT
#include <nvtt/nvtt.h>
#endif

class TextureCube : public BaseTexture
{
  std::string skybox_root;
public:
  TextureCube();
  TextureCube(std::string name);
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

  // Inherited via BaseTexture
  virtual void setUnit(uint unit) override;
};