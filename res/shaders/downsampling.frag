#version 330 core

#define OFFSET vec2(0,0)

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D image;

uniform bool horizontal;
uniform float weight[2] = float[](0.125, 0.5);

// Todo: specify calculation of offset for box filter
vec3 offsets[13] = vec3[](
	vec3(1,1, weight[0]),
	vec3(3,1, weight[0]),
	vec3(5,1, weight[0]),

	vec3(2,2, weight[1]),
	vec3(4,2, weight[1]),
	
	vec3(1,3, weight[0]),
	vec3(3,3, weight[0]),
	vec3(5,3, weight[0]),
	
	vec3(2,4, weight[1]),
	vec3(4,4, weight[1]),

	vec3(1,5, weight[0]),
	vec3(3,5, weight[0]),
	vec3(5,5, weight[0])

);

vec4 Sample(vec2 uv) {
	return texture(image, uv);
}

void main()
{             
	vec2 tex_offset = 1.0 / textureSize(image, 0); // gets size of single texel
	vec4 result = vec4(0);

	for (int i = 0; i < 13; i++)
	{
		result += Sample(TexCoords + offsets[i].xy*tex_offset) * offsets[i].z;
	}
	FragColor = vec4(result);
}