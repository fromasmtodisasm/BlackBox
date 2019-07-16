#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;


out vec3 TexCoords;

uniform mat4 Projection;
uniform mat4 View;

void main()
{
    TexCoords = aPos;
    gl_Position = Projection * View * vec4(aPos, 1.0);
}  