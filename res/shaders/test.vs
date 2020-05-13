#version 330 core
#include "common.h"

#pragma attribute(position, aPos)
#pragma attribute(tc, aTexCoords)

void main()
{
	gl_Position = vec4(aPos, 1);
}