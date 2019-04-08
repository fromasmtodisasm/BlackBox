#version 330 core
uniform vec3 color;
out vec4 u_color;

void main()
{
    u_color = vec4(color,1.0f);//vec4(1,0,0,1);
    //color = vertexColor;
} 
