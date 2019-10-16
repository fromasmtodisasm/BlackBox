#pragma once
#include <BlackBox/IPostProcessor.hpp>
#include <BlackBox/Render/Shader.hpp>
#include <BlackBox/Quad.hpp>

class PostProcessor : public IPostProcessor
{
public:
	PostProcessor(const char *shader);
	~PostProcessor();

private:
	BaseShaderProgramRef m_ScreenShader;
	Quad m_ScreenQuad;

	// Inherited via IPostProcessor
	virtual void Do(unsigned int texture) override;

};
