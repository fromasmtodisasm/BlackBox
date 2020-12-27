GLSLShader
{
    #version 460 core
}

GLSLShader vert
{
    out vec2 TexCoords;

    void main()
    {
        gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); 
        TexCoords = aTexCoords;
    }
}

GLSLShader frag
{
    out vec4 FragColor;
    
    in vec2 TexCoords;

    uniform sampler2D screenTexture;

    void main()
    { 
        FragColor = texture(screenTexture, TexCoords);
    }
}

technique main {
    pass p0
    {
        InputLayout
        {
            vec3 aPos : POSITION
            vec2 aTC : TEXCOORD
        }
        VertexShader = vert
        PixelShader = frag
    }
}