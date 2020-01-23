#pragma once
#if 1
#include <BlackBox/Renderer/BaseShader.hpp>
#include <BlackBox/System/ISystem.hpp>
#include <string>
#include <memory>

class CShader;
class CShaderProgram;

class CShaderProgram : public CBaseShaderProgram {
public:
  CShaderProgram();
  CShaderProgram(ShaderRef vs, ShaderRef fs);
  CShaderProgram(ShaderInfo& vs, ShaderInfo& fs);
  CShaderProgram(ShaderInfo& vs, ShaderInfo& fs, ShaderInfo& gs);
  CShaderProgram(ShaderInfo& vs, ShaderInfo& fs, ShaderInfo& gs, ShaderInfo& cs);
  //CShaderProgram(std::string vs, std::string fs);
  ~CShaderProgram();
  virtual void setup() override;
};
#endif