Language GLSL
/*
#if 0
Extension
{
    GL_ARB_bindless_texture : require
}
#endif
*/
#include "common.fx"
GLSLShader
{
    uniform layout(std140, binding = 15) SpriteBuffer
    {
        mat4 projection;
        mat4 model;
        mat4 uv_projection;
        vec3 textColor;
        vec2 uv;
    };

}
GLSLShader vert
{
    out layout(location = 0) vec2 TexCoords;
    out layout(location = 1) vec4 Color;

    void main()
    {
        gl_Position = vec4(aPos, 1.0);
        TexCoords = vec4(aTex, 0.f, 1.0).xy;
        Color = aCol;
    }  
}

GLSLShader frag
{
    in layout(location = 0) vec2 TexCoords;
    in layout(location = 1) vec4 Color;
    out layout(location = 0) vec4 color;

    uniform layout(binding = 0) sampler2D text;

    void main()
    {    
        vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
        color = vec4(Color) * sampled;
    }
}

Technique main {
    Pass p0
    {
        InputLayout
        {
            vec3 aPos : POSITION
            vec4 aCol : COLOR
            vec2 aTex : TEXCOORD
        }
        VertexShader = vert
        PixelShader = frag
    }
}