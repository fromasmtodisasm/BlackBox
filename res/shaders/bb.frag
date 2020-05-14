#version 330 core
out vec4 FragColor;

uniform float alpha;
uniform vec4 color;
  
void main()
{ 
	FragColor = color / 255.0;
}