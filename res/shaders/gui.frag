#if 0
#if __VERSION == 120
#ifdef GL_ES
	precision mediump float;
#endif
uniform sampler2D Texture;
varying vec2 Frag_UV;
varying vec4 Frag_Color;
void main()
{
	gl_FragColor = Frag_Color * texture2D(Texture, Frag_UV.st);
};

#elseif __VERSION == 130
uniform sampler2D Texture;
in vec2 Frag_UV;
in vec4 Frag_Color;
out vec4 Out_Color;
void main()
{
	Out_Color = Frag_Color * texture(Texture, Frag_UV.st);
};
#elseif __VERSION >= 410
#else
#version 410 core
in vec2 Frag_UV;
in vec4 Frag_Color;
uniform sampler2D Texture;
layout (location = 0) out vec4 Out_Color;
void main()
{
	Out_Color = Frag_Color * texture(Texture, Frag_UV.st);
};
#endif
#endif


