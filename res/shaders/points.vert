#version 330 core
#include "common.h"

#pragma attribute(position, pos)

uniform mat4 MVP;
uniform int point_size = 1;

void main()
{
	gl_PointSize = point_size;
	gl_Position = MVP * vec4(position, 1);
}