#version 330 core
#include "common.h"

#pragma attribute(position, aPos)
#pragma attribute(normal, aN)

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 vN;
out vec3 vWPos;
out vec3 diffuse;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vec3 lightPos = vec3(10,10,10);
    vec3 lightVector = lightPos - model * vec4(aPos, 1.0);
    diffuse = max(0, dot(lightVecotr, aN));
}