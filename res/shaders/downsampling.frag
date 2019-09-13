#version 330 core

#define OFFSET vec2(0,0)

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D image;

uniform bool horizontal;
uniform float weight[5] = float[](0.125f, 0.125f, 0.5, 0.125f, 0.125f);

vec4 Sample(vec2 uv) {
	return texture2D(image, uv);
}

vec4 SampleBox(vec2 uv) 
{
	//vec4 o = (1.0 / textureSize(image, 0)).xyxy * vec2(-1, 1).xxyy;
	vec2 tex_offset = (1.0 / textureSize(image, 0));
	vec3 result;
	/*
	vec4 s =
		Sample(uv + o.xy) + Sample(uv + o.zy) +
		Sample(uv + o.xw) + Sample(uv + o.zw);
	*/
	result += Sample(uv + vec2(0.0, 0.0)).rgb;
	result += Sample(uv + vec2(tex_offset.x, 0.0)).rgb;
	result += Sample(uv + vec2(0.0, tex_offset.y)).rgb;
	result += Sample(uv + vec2(tex_offset.x, tex_offset.y)).rgb;
	result *= 0.25;
	return vec4(result,1);
}

void main()
{             
	vec2 tex_offset = 1.0 / textureSize(image, 0); // gets size of single texel

	vec4 result = SampleBox(TexCoords);
	result = SampleBox(TexCoords + OFFSET);
	result = SampleBox(TexCoords + OFFSET);
	result = SampleBox(TexCoords + OFFSET);
	result = SampleBox(TexCoords + OFFSET);




	FragColor = vec4(result);
}