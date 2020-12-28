FatalToken
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
	}perViewCB;

    struct Light
    {
        vec4 position;
    }

	layout(std140, binding = 3) uniform Lights {
        Light l[16];
	}lights;
}


technique Common
{
  pass p0
  {
    VertexShader = vert
    PixelShader = frag
  }
}