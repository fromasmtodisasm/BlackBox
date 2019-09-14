#version 330 core

#define OFFSET vec2(0,0)

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D image;

uniform bool horizontal;
uniform float weight[5] = float[](0.125f, 0.125f, 0.5, 0.125f, 0.125f);

// Todo: specify calculation of offset for box filter
vec2 offsets[5] = vec2[](
	vec2(0),
	vec2(0),
	vec2(0),
	vec2(0),
	vec2(0)
);

vec4 Sample(vec2 uv) {
	return texture2D(image, uv);
}

vec4 SampleBox(vec2 uv) 
{
	vec2 tex_offset = (1.0 / textureSize(image, 0));
	vec3 result;
	
	// Todo: specify calculation of offset for box filter
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
	vec4 result = vec4(0);

	for (int i = 0; i < 5; i++)
	{
		result += SampleBox(TexCoords + offsets[i]) * weight[i];
	}
	FragColor = vec4(result);
}