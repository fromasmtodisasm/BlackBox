#pragma once
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Renderer/IShader.hpp>

struct IShaderManager
{
  virtual ShaderProgramRef getProgram(const char* vShader, const char* fShader) = 0;
  virtual ShaderProgramRef getProgram(const char* vShader, const char* fShader, const char* gShader) = 0;
  virtual ShaderProgramRef getProgram(const char* vShader, const char* fShader, const char* gShader, const char* cShader) = 0;
  virtual ShaderProgramRef getProgram(IShaderProgram::ShaderInfo& vs, IShaderProgram::ShaderInfo& fs, IShaderProgram::ShaderInfo& gs, IShaderProgram::ShaderInfo& cs) = 0;
  virtual ShaderProgramRef loadProgram(ProgramDesc& desc, bool isReload) = 0;
  virtual ShaderProgramRef loadProgram(const char* name, int flags) = 0;
  virtual ShaderProgramRef getDefaultProgram() = 0;
  virtual ShaderRef getShader(ShaderDesc const& desc, bool isReload) = 0;
  virtual ShaderRef addShader(const ShaderDesc& desc) = 0;
  virtual void removeShader(const char* name) = 0;
};