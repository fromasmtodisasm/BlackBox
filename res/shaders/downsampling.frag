#version 450 core

#define OFFSET vec2(0,0)

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D image;

uniform bool horizontal;
uniform float weight[2] = float[](0.125, 0.5);
uniform float offset = -3.5;
uniform vec2 viewPort = vec2(1, 1);

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

//vec4 Sample(vec2 uv) {
//	return texture(image, uv);
//}

#define Sample(uv) texture(image, uv)

vec4 downsample()
{
	vec4 result = vec4(0);
	vec2 tex_offset = 1.0 / textureSize(image, 0); // gets size of single texel

	//return Sample(TexCoords);
	for (int i = 0; i < 13; i++)
	{
		vec2 texel = TexCoords + (offsets[i].xy + offset)*tex_offset;
		result += Sample(texel * viewPort) * offsets[i].z;
	}
	return result;
}

void main()
{    
	vec4 result = vec4(0);
	vec2 tex_offset = 1.0 / textureSize(image, 0); // gets size of single texel

	//return Sample(TexCoords);
	for (int i = 0; i < 13; i++)
	{
		vec2 texel = TexCoords + (offsets[i].xy + offset)*tex_offset;
		result += Sample((TexCoords + (offsets[i].xy + offset)*tex_offset)*viewPort) * offsets[i].z;
	}
	//return result;         
	FragColor = result;
}