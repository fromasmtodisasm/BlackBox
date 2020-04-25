#version 330 core
#include "common.h"
POSITION_ATTR(aPos);
TC_ATTR(aTex);
out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 uv_projection;

void main()
{
    gl_Position = projection * model * vec4(aPos, 1.0);
    TexCoords = (uv_projection * vec4(aTex, 0.f, 1.0)).xy;
}  
