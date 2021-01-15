#pragma once
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Renderer/IShader.hpp>

struct IShaderManager
{
  virtual ShaderRef getShader(ShaderDesc const& desc, bool isReload) = 0;
  virtual ShaderRef addShader(const ShaderDesc& desc) = 0;
  virtual void removeShader(const char* name) = 0;
};