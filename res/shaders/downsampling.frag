#version 450 core
//layout (pixel_center_integer) in vec4 gl_FragCoord;

#define OFFSET vec2(0,0)

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D image;

uniform bool horizontal;
uniform float weight[2] = float[](0.125, 0.5);
uniform float offset = -3.0;
uniform float vx = 1;
uniform float vy = 1;

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

/*
	TexCoords - coordinates current pixel in out framebuffer
	image - n + 1 mip level
	texel_scale - step in relative units in input image -- key of sampling
*/
vec4 downsample()
{
	vec4 result = vec4(0);
	vec2 texel_scale = 1.0 / textureSize(image, 0); // gets size of single texel

	for (int i = 0; i < 13; i++)
	{
		//vec2 texel = ((gl_FragCoord.xy * 2 + offsets[i].xy + offset) / textureSize(image, 0)) * viewPort;
		vec2 texel = clamp((gl_FragCoord.xy + offsets[i].xy + offset)*2 / vec2(textureSize(image, 0))*vec2(vx,vy), vec2(0),vec2(1));
		//vec2 texel = TexCoords;
		result += Sample(texel) * offsets[i].z;
	}
	return result;
}

void main()
{    
	FragColor = downsample();
}