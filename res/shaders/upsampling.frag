#version 330 core

///////////////////////
out vec4 FragColor;

///////////////////////
in vec2 TexCoords;

///////////////////////
uniform sampler2D current;
uniform sampler2D blured;
uniform bool blurOn = true;
uniform bool blurOnly = true;
// viewport
uniform float vx = 0;
uniform float vy = 0;
uniform int rx = 1;
uniform int ry = 1;
//

float weight[9] = float[](
	1,2,1,	
	2,4,2,	
	1,2,1	
);

vec4 blur(vec2 uv)
{
	vec4 result = vec4(0);
	vec2 tex_offset = 1.0 / textureSize(blured, 0);
	int divisor = 0;
	float w = 0;
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1, index = (i+1)*(j+1) + j + 1; j <= 1; j++)
		{
			float dev = 2;
			w += weight[index];
			vec2 coord = uv + vec2(j, i);
			//vec2 texel = clamp(coord, vec2(1),	0.5*vec2(rx,ry)-1) * tex_offset;
			vec2 texel = coord * tex_offset;
			/*
			if (all(greaterThan(uv + vec2(j,i),	0.5*vec2(rx,ry)-1)))
			{
				vec2 coord = coord - 1;
				texel = clamp(coord, vec2(1),	0.5*vec2(rx,ry)-1) * tex_offset;
				result += vec4(textureLod(blured, texel, 0).rgb * weight[index], 1);  
				continue;
			}
			*/
			result += vec4(textureLod(blured, texel, 0).rgb * weight[index], 1);  
		}
	}
	return result / 16.0;
}

void main()
{             
	vec2 texel = gl_FragCoord.xy / vec2(textureSize(current, 0));
	vec2 uv = 0.5 * (gl_FragCoord.xy) - 0.5;
	if (blurOnly)
	{
		FragColor = blur(uv);
	}
	else 
	{
		FragColor = textureLod(current, clamp(texel, vec2(0), vec2(vx,vy)), 0) + blur(uv);
	}
}


