#version 330 core
in vec2 UV;
out vec4 FragColor;

uniform sampler2D quad;

void main()
{
  FragColor = vec4(texture2D(quad, UV), 1);
}
