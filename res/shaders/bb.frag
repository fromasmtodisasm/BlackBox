#version 330 core
#include <test.glsl>
#include <test2.glsl>
out vec4 FragColor;
  
void main()
{ 
	FragColor = LineColor;//vec4(1.0f, 1.0f, 0.0f, 1.0f);
}