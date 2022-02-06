GLSLShader {
    #version 410 core
}
GLSLShader frag {

    in vec2 Frag_UV;
    in vec4 Frag_Color;
    uniform sampler2D Texture;
    layout (location = 0) out vec4 Out_Color;
    void main()
    {
        Out_Color = Frag_Color * texture(Texture, Frag_UV.st);
    };
}

GLSLShader vert
{
    uniform mat4 ProjMtx;
    out vec2 Frag_UV;
    out vec4 Frag_Color;
    void main()
    {
        Frag_UV = UV;
        Frag_Color = Color;
        gl_Position = ProjMtx * vec4(Position.xy,0,1);
    };

}

technique Main
{

  pass p0
  {
	InputLayout
	{
		vec3 Position : POSITION
		vec4 Color : COLOR
		vec2 UV : TEXCOORD
	}
    VertexShader = vert
    PixelShader = frag
  }
}