#version 330 core

layout (location = 0) in vec3 Pos;
layout (location = 2) in vec2 TexCoords;

out vec2 UV;

void main()
{
  gl_Position = vec4(Pos, 1); 
  UV = TexCoords;
}
 