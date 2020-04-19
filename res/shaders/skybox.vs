#version 330 core
#include "common.h"

#pragma attribute(position, aPos)

out vec3 TexCoords;

uniform mat4 Projection;
uniform mat4 View;

void main()
{
	TexCoords = aPos;
	vec4 pos = Projection * View * vec4(aPos, 1.0);
    gl_Position = pos.xyww; 
}  