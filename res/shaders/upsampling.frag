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
uniform float rx = 1;
uniform float ry = 1;
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
			vec2 m = vec2(vx,vy) - 1*tex_offset;
			//if (all(lessThan((0.5*gl_FragCoord.xy + vec2(j, i)), vec2(0.5*floor(rx) - dev,0.5*floor(ry) - dev))))
			//{
				w += weight[index];
				//vec2 texel = clamp((uv + vec2(j, i) * tex_offset), vec2(0.5)*tex_offset, m);
				vec2 coord = uv + vec2(j, i);
				vec2 texel = round(clamp(coord, vec2(0.5),	0.5*floor(vec2(rx-1,ry-1))-1)) * tex_offset;
				if (all(greaterThan(uv + vec2(j,i),			0.5*floor(vec2(rx-1,ry-1))-1)))
					continue;
				result += vec4(texture(blured, texel).rgb * weight[index], 1);  
			//}
			if (1==0)		
			{
				vec2 texel = clamp(vec2(uv), vec2(0.5)*tex_offset, 0.5*(gl_FragCoord.xy - 2)/textureSize(blured,0));
				float anti_overexposure = 1;
				result += vec4(texture(blured, texel).rgb * weight[index], 1)*anti_overexposure;  
			}
			
		}
	}
	//return result / (divisor == 0 ? 1 : divisor);
	return result / 16.0;
	//return result / w;
}

void main()
{             
	vec2 texel = gl_FragCoord.xy / vec2(textureSize(current, 0));
	vec2 uv = 0.5 * (gl_FragCoord.xy);
	if (blurOnly)
	{
		FragColor = blur(uv);
	}
	else 
	{
		FragColor = texture(current, clamp(texel, vec2(0), vec2(vx,vy))) + blur(uv);
	}
}

