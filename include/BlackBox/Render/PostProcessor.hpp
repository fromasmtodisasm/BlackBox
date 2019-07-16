#pragma once
#include <BlackBox/IPostProcessor.hpp>
#include <BlackBox/Render/CShader.hpp>
#include <BlackBox/Quad.hpp>

class PostProcessor : public IPostProcessor
{
public:
	PostProcessor(const char *shader);
	~PostProcessor();

private:
	CShaderProgram* m_ScreenShader;
	Quad m_ScreenQuad;

	// Inherited via IPostProcessor
	virtual void Do(unsigned int texture) override;

};
