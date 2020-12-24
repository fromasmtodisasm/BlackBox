GLSLShader
{
	#version 330 core
	#if 0
	layout(binding = 0) uniform MatrixBlock
	{
	  mat4 projection;
	  mat4 view;
	  mat4 model;
	};

	//layout(location = 0) out vec4 v_color;
	#else

	uniform mat4 projection;
	uniform mat4 view;
	uniform mat4 model;
	#endif

}

/*
InputLayout
{
	vec3 aPos : POSITION;
	vec3 aColor : COLOR;
	vec2 aTC : TEXCOORD;
}
*/

GLSLShader vert
{

    layout (location = 0) in vec3 aPos;
    layout (location = 2) in vec3 aTC;
    layout (location = 5) in vec4 aColor;
	out vec4 v_color;

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
    VertexShader = vert
    PixelShader = frag
  }
  /*
  Pass p1
  {
    VertexShader = vert
    PixelShader = frag
  }
  */
}
