#pragma once
#include <BlackBox/Render/Opengl.hpp>
#include <BlackBox/Render/CBaseShader.hpp>
#include <BlackBox/IEngine.hpp>
#include <glm/fwd.hpp>
#include <string>
#include <memory>

class CShader; 
class CShaderProgram;

class CShaderProgram : public CBaseShaderProgram{
public:
	CShaderProgram();
	CShaderProgram(ShaderRef vs, ShaderRef fs);
	CShaderProgram(ShaderInfo &vs, ShaderInfo &fs);
	CShaderProgram(std::string vs, std::string fs);
	~CShaderProgram();
	virtual void setup() override;
};
