#version 330 core
#include "common.h"

#pragma attribute(position, aPos)
#pragma attribute(normal, aNormal)

out vec3 WorldPos;
out vec3 Normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;


void main()
{
    
    WorldPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(model) * aNormal;   

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
