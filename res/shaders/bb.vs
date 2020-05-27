#version 330 core
#include "common.h"

#pragma attribute(position, aPos)
#pragma attribute(normal, aN)

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out VS_OUT
{
	vec3 fragPos;
	vec3 N;
}vs_out;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vs_out.fragPos = vec3(model * vec4(aPos, 1.0));
    vs_out.N = aN;
}
