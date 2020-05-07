#include <BlackBox/Renderer/GBuffer.hpp>
#include <BlackBox/Renderer/OpenGL/Core.hpp>
#include <BlackBox/Renderer/MaterialManager.hpp>
#include <BlackBox/Renderer/IRender.hpp>

GBuffer::GBuffer(size_t SRC_WIDTH, size_t SRC_HEIGHT)
{
#if 0
	// positions buffer
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SRC_WIDTH, SRC_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

	// normals buffer
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SRC_WIDTH, SRC_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

	// albedo + specular buffer
	glGenTextures(1, &gAlbedoSpec);
	glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SRC_WIDTH, SRC_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);

	//  which buffers we will use during rendering
	unsigned int attachments[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
	glDrawBuffers(3, attachments);

	ProgramDesc desc[] = {
		{"geometry_pass",
		 ShaderDesc("geometry_pas.vs"),
		 ShaderDesc("geometry_pas.frag")},
		{"lighting_pass",
		 ShaderDesc("lighting_pass.vs"),
		 ShaderDesc("lighting_pass.frag")},
	};
  MaterialManager::instance()->loadProgram(desc[0], false);
  MaterialManager::instance()->loadProgram(desc[1], false);

  shaderGeometryPass = MaterialManager::instance()->getProgram(desc[0].name);
  shaderLightingPass = MaterialManager::instance()->getProgram(desc[1].name);
#endif
}

void GBuffer::GeometryPass()
{
}

void GBuffer::LightPass(Vec3 viewPos)
{
#if 0
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);

	shaderLightingPass->Use();
	shaderLightingPass->BindTextureUnit2D(gPosition, 0);
	shaderLightingPass->BindTextureUnit2D(gNormal, 1);
	shaderLightingPass->BindTextureUnit2D(gAlbedoSpec, 2);
	shaderLightingPass->Uniform(viewPos, "viewPos");

	gEnv->pRenderer->DrawFullscreenQuad();
#endif
	
}
