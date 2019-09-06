#version 330 core

out vec4 Fragcolor;
uniform vec3 color = vec3(0,4,0);
uniform vec3 emissive = vec3(10, 9, 8);

void main()
{
	Fragcolor = vec4(color, 1) * vec4(emissive, 1);	
}
