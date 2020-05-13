#version 330 core
#include "common.h"

#pragma attribute(position, aPos)
#pragma attribute(tc, aTexCoords)

out vec2 TexCoords;
uniform mat4 projection = mat4(1.0);
uniform mat4 model = mat4(1.0);
uniform mat4 uv_projection = mat4(1.0);

void main()
{
    gl_Position = projection * model * vec4(aPos, 1.0f); 
    //gl_Position = vec4(aPos, 1.0f); 
    //TexCoords = (uv_transform * vec3(aTexCoords, 1.0)).xy;
    //TexCoords = (uv_projection * vec4(aTexCoords, 1.0, 1.0)).xy;
    TexCoords = aTexCoords;
}
