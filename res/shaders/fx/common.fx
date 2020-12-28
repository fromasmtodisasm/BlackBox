GLSLShader
{
	#version 460 core
}

GLSLShader
{
	layout(std140, binding = 0) uniform PerFrameCB {
        float deltaTime;
	}perFrameCB;

	layout(std140, binding = 2) uniform PerViewCB {
		mat4 projection;
		mat4 ortho_projection;
		mat4 view;
		mat4 view_proj;
	}perViewCB;

	mat4 GetOrthoProjMat()
	{
		return perViewCB.ortho_projection;
	}

	mat4 GetProjMat()
	{
		return perViewCB.projection;
	}

	mat4 GetViewProjMat()
	{
		return perViewCB.view_proj;
	}
}