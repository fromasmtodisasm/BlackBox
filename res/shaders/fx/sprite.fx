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
    out layout(location = 10) vec2 TexCoords;

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

    uniform layout(binding = 0) sampler2D text;

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