#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float alpha = 0.9;
uniform vec4 color;
uniform vec4 screen;

void main()
{ 
    //vec2 uv = screen.zw * gl_FragCoord.xy; 
    vec2 uv = TexCoords; 
    FragColor = vec4(texture(screenTexture, uv)*color);
	//FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}