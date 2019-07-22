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
	CShaderProgram(CShader* vs, CShader* fs);
	~CShaderProgram();
	virtual void setup() override;
};
