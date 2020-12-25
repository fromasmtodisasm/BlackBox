GLSLShader vert
{
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 2) in vec2 aTexCoords;

    out vec2 TexCoords;

    uniform mat4 projection = mat4(1.0);
    uniform mat4 model = mat4(1.0);
    uniform mat4 uv_projection = mat4(1.0);

    void main()
    {
        gl_Position = projection * model * vec4(aPos, 1.0f); 
        TexCoords = (uv_projection * vec4(aTexCoords, 1.0, 1.0)).xy;
    }

}

GLSLShader frag
{
    #version 330 core
    out vec4 FragColor;
    
    in vec2 TexCoords;

    uniform sampler2D screenTexture;
    uniform float alpha = 0.9;
    uniform vec4 color;
    uniform vec4 screen;

    void main()
    { 
        vec2 uv = TexCoords; 
        FragColor = vec4(texture(screenTexture, uv)*color);
    }

}

Technique main
{
    Pass p0
    {
        VertexShader = vert
        PixelShader = frag
    }
}
