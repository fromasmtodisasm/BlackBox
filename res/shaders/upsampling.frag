#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D current;
uniform sampler2D previos;

float weight[9] = float[](
	1,2,1,	
	2,4,2,	
	1,2,1	
);

vec4 blur(vec2 uv)
{
	vec4 result = vec4(0);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0, index = i*j + j; j < 3; j++)
		{
			result += vec4(texture(previos, uv + vec2(j, i)).rgb * weight[index], 1);  
		}
	}
	return result / 16.0;
}

void main()
{             
	vec2 tex_offset = 1.0 / textureSize(previos, 0); // gets size of single texel
	vec4 result = vec4(0);

	FragColor = texture(current, TexCoords) + blur(TexCoords);
}