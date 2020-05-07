#pragma once
#include <BlackBox/Renderer/IRender.hpp>

struct IShaderManager
{
  virtual ShaderProgramRef getProgram(const char* vShader, const char* fShader) = 0;
  virtual ShaderProgramRef getProgram(const char* vShader, const char* fShader, const char* gShader) = 0;
  virtual ShaderProgramRef getProgram(const char* vShader, const char* fShader, const char* gShader, const char* cShader) = 0;
  virtual ShaderProgramRef getDefaultProgram() = 0;
  virtual ShaderRef getShader(ShaderDesc const& desc, bool isReload) = 0;
  virtual ShaderRef addShader(const ShaderDesc& desc) = 0;
  virtual void removeShader(const char* name) = 0;
};