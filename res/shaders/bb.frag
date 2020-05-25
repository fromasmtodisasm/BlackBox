#version 330 core
out vec4 FragColor;

uniform float alpha;
uniform vec4 color;
  
in vec3 diffuse;
void main()
{ 
	//FragColor = color / 255.0;
	FragColor = vec4(diffuse, 1);
}