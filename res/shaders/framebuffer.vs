#version 330 core
#include "common.h"

#pragma attribute(position, aPos)
#pragma attribute(tc, aTexCoords)

out vec2 TexCoords;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); 
    TexCoords = aTexCoords;
}
