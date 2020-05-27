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

in VS_OUT
{
	vec3 fragPos;
	vec3 N;
}vs_out;
  
void main()
{ 
	//FragColor = color / 255.0;
	vec4 p = gl_FragCoord;
	FragColor = vec4(
		tonemap(ambient() + diffuse(lightPos, vs_out.fragPos, vs_out.N) + specular(lightPos, vs_out.fragPos, eye, vs_out.N)
	), 1);
}