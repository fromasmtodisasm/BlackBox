Language GLSL
#include "common.fx"
GLSLShader vert
{
    out layout(location = 10) vec2 TexCoords;

    uniform layout(location = 0) mat4 projection;
    uniform layout(location = 1) mat4 model;
    uniform layout(location = 2) mat4 uv_projection;

    void main()
    {
        gl_Position = projection * model * vec4(aPos, 1.0);
        TexCoords = (uv_projection * vec4(aTex, 0.f, 1.0)).xy;
    }  
}

GLSLShader frag
{
    in layout(location = 10) vec2 TexCoords;
    out layout(location = 0) vec4 color;

    uniform sampler2D text;
    uniform layout(location = 3) vec3 textColor;

    void main()
    {    
        vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
        color = vec4(textColor, 1.0) * sampled;
    }
}

Technique main {
    Pass p0
    {
        InputLayout
        {
            vec3 aPos : POSITION
            vec2 aTex : TEXCOORD
        }
        VertexShader = vert
        PixelShader = frag
    }
}