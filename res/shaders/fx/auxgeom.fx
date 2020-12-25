GLSLShader
{
	#version 460 core
}

GLSLShader vert
{

	out vec4 v_color;

	uniform mat4 projection;
	uniform mat4 view;
	uniform mat4 model;

	void main()
	{
		gl_Position = projection * view * vec4(aPos, 1.0);
		v_color = vec4(aColor);
	} 
}

GLSLShader frag
{
	out vec4 FragColor;
	in vec4 v_color;
	void main()
	{ 
		FragColor = vec4(v_color);
	}
}

// Default technique for auxiliary geometry rendering
Technique AuxGeometry
{

  Pass p0
  {
	InputLayout
	{
		vec3 aPos : POSITION
		vec4 aColor : COLOR
		vec2 aTC : TEXCOORD
	}
    VertexShader = vert
    PixelShader = frag
  }
  Pass p1
  {
    VertexShader = vert
    PixelShader = frag
  }
}

Technique Test
{
  Pass p0
  {
    VertexShader = vert
    PixelShader = frag
  }
}