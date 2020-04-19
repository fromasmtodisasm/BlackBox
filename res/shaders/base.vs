#version 330 core
#include "common.h"

#pragma attribute(position, aPos)

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;


void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
} 