Language GLSL
#include "common.fx"

GLSLShader{
	layout(std140, binding = 11) uniform ScreenBuffer
    {
        mat4 model;
        mat4 uv_projection;
        vec4 color;
        float alpha;
	};

}

GLSLShader vert
{
    out  layout(location = 10) vec2 TexCoords;
    void main()
    {
        gl_Position = GetOrthoProjMat() * model * vec4(aPos, 1.0f); 
        TexCoords = (uv_projection * vec4(aTexCoords, 1.0, 1.0)).xy;
    }
}

GLSLShader frag
{
    in layout(location = 10) vec2 TexCoords;
    out layout(location = 0) vec4 FragColor;

    uniform sampler2D screenTexture;

    void main()
    { 
        vec2 uv = TexCoords; 
        FragColor = vec4(texture(screenTexture, uv))*vec4(color.rgb, alpha);
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

