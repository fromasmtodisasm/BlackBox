#version 330 core

in vec4 vertexColor; // Input variable from vertex shader 
out vec4 color;

void main()
{
    color = vertexColor;
} 
