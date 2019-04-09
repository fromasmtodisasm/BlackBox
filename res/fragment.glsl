#version 330 core
out vec4 u_color;
uniform vec4 color = vec4(1.0, 0.0, 0.0, 1.0);

void main()
{
    u_color = color; 
} 
