#version 330 core
out vec4 FragColor;
#define TM_UNREAL
#include "lighting.h"

uniform float alpha;
uniform vec4 color;
uniform vec3 lightPos;
uniform vec3 eye;
uniform int fid;
uniform mat4 projection;

uniform bool bTonemap;

#define dbgDIFFUSE 1
#define dbgSPECULAR 2
#define dbgNORMAL 3

uniform int dbgmode = 0;

in VS_OUT
{
	vec3 fragPos;
	vec3 N;
}vs_out;

struct OutColor
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 normal;
};
  
void main()
{ 
	struct OutColor outColor;
	outColor.ambient = vec3(0);
	outColor.diffuse = vec3(0);
	outColor.specular = vec3(0);

	vec4 p = gl_FragCoord;
	vec3 _color = vec3(color);


	outColor.ambient = ambient()*_color;
	outColor.diffuse = vec3(diffuse(lightPos, vs_out.fragPos, vs_out.N));//*_color;
	outColor.specular = 0.8*_color*specular(lightPos, vs_out.fragPos, eye, vs_out.N);

	vec3 result = outColor.ambient + outColor.diffuse + outColor.specular;

	switch (dbgmode)
	{
	case 0:

		break;
	case dbgDIFFUSE:
		result = outColor.diffuse + ambient();	
		break;
	case dbgNORMAL:
		result = 0.5*(normalize(vs_out.N) + 1);	
		break;
	case dbgSPECULAR:
		result = outColor.specular;	
		break;
	}

	if (bTonemap)
	{
		FragColor = vec4(tonemap(result), 1);
	}
	else
	{
	FragColor = vec4(result, 1);
	}
}
