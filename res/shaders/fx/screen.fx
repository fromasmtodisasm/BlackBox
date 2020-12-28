GLSLShader 
{
    #version 460 core
}

GLSLShader vert
{
    out  layout(location = 10) vec2 TexCoords;

    uniform layout(location = 1) mat4 model = mat4(1.0);
    uniform layout(location = 2) mat4 uv_projection = mat4(1.0);

	layout(std140, binding = 2) uniform Matricies {
		mat4 projection;
        mat4 ortho_projection;
		mat4 view;
	};

    void main()
    {
        gl_Position = ortho_projection * model * vec4(aPos, 1.0f); 
        TexCoords = (uv_projection * vec4(aTexCoords, 1.0, 1.0)).xy;
    }



}

GLSLShader frag
{
    out layout(location = 0) vec4 FragColor;
    
    in layout(location = 10) vec2 TexCoords;

    uniform sampler2D screenTexture;

    uniform layout(location = 3) float alpha = 0.9;
    uniform layout(location = 4) vec4 color;

    void main()
    { 
        vec2 uv = TexCoords; 
        FragColor = vec4(texture(screenTexture, uv)*color);
    }

}

technique main
{
    pass p0
    {
        InputLayout
        {
            vec3 aPos : POSITION
            vec2 aTexCoords : TEXCOORD
        }

        VertexShader = vert
        PixelShader = frag
    }
}
