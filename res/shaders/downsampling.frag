#version 450 core
//layout (pixel_center_integer) in vec4 gl_FragCoord;
///////////////////////
out vec4 FragColor;

///////////////////////
in vec2 TexCoords;

///////////////////////
uniform sampler2D image;
uniform bool horizontal;
uniform float weight[2] = float[](0.125, 0.5);
uniform float offset = -3.0;
uniform float vx = 1;
uniform float vy = 1;
uniform int rx;
uniform int ry;

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

#define Sample(uv) textureLod(image, uv, 0)

vec4 get_texel(int i)
{
	vec4 result = vec4(0);
	vec2 tex_size = 1.0 / textureSize(image, 0); // gets size of single texel

	vec2 coord = (2 * (gl_FragCoord.xy)+ (offsets[i].xy + offset));
	float dev = 0.5;
	vec2 m = vec2(vx,vy) - tex_size;
	vec2 texel = clamp(coord, vec2(1), vec2(rx,ry) - 1) * tex_size;
	if (all(greaterThan(coord, vec2(rx,ry)-1)))
	{
		vec2 coord = (2 * (gl_FragCoord.xy)+ (offsets[i].xy + 2*offset));
		texel = floor(clamp(coord, vec2(1), vec2(rx,ry) - 1)) * tex_size;
		result = Sample(texel) * offsets[i].z;
	}
	else
	{
		result = Sample(texel) * offsets[i].z;
	}

	return result;
}

/*
	TexCoords - coordinates current pixel in out framebuffer
	image - n + 1 mip level
	texel_scale - step in relative units in input image -- key of sampling
*/
vec4 downsampling(vec2 uv)
{
	vec4 result = vec4(0);
	#if 0
	for (int i = 0; i < 13; i++)
	{
		result += get_texel(i);	
	}
	#else
	result += get_texel(0);
	result += get_texel(1);
	result += get_texel(2);
	result += get_texel(3);
	result += get_texel(4);
	result += get_texel(5);
	result += get_texel(6);
	result += get_texel(7);
	result += get_texel(8);
	result += get_texel(9);
	result += get_texel(10);
	result += get_texel(11);
	result += get_texel(12);
	#endif
	return result;
}

void main()
{    
	vec2 uv = 2 * (gl_FragCoord.xy) / vec2(textureSize(image, 0));
	FragColor = downsampling(uv);
}

