#version 330 core

layout (location = 0) in vec3 position;


uniform mat4 MVP;
uniform int point_size = 1;

void main()
{
	gl_PointSize = point_size;
	gl_Position = MVP * vec4(position, 1);
}