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
	CShaderProgram(std::shared_ptr<CShader> vs, std::shared_ptr<CShader> fs);
	CShaderProgram(std::string vs, std::string fs);
	~CShaderProgram();
	virtual void setup() override;
};
