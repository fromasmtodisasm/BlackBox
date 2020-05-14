#version 330 core
#include "common.h"

#pragma attribute(position, aPos)
#pragma attribute(color, aColor)
#pragma attribute(tc, aTC)

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec4 v_color;

void main()
{
    //gl_Position = projection * view * model * vec4(aPos, 1.0);
    gl_Position = projection * view * vec4(aPos, 1.0);
    //gl_Position = vec4(aPos, 1.0);
	//v_color = aColor;
	v_color = vec4(aColor);
} 
