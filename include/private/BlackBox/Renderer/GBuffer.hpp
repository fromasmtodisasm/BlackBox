#include <BlackBox/Renderer/Shader.hpp>

class GBuffer
{
public:
  unsigned int gPosition, gNormal, gAlbedoSpec;
  BaseShaderProgramRef shaderGeometryPass;
  BaseShaderProgramRef shaderLightingPass;
	GBuffer(size_t SRC_WIDTH, size_t SRC_HEIGHT);

	void GeometryPass();
	void LightPass(Vec3 viewPos);

};