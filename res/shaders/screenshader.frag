#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float alpha = 0.9;

void main()
{ 
    FragColor = vec4(texture(screenTexture, TexCoords).rgb, alpha);
	//FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}