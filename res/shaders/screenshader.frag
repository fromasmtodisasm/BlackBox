#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float alpha = 0.9;
uniform vec4 color;

void main()
{ 
	//vec4 color = vec4(0,1,0,1);
    FragColor = vec4(texture(screenTexture, TexCoords)*color);
	//FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}