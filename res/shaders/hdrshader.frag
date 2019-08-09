#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float alpha = 0.9;

uniform float exposure = 1.0;

void main()
{
    const float gamma = 2.2;
    vec3 hdrColor = texture(screenTexture, TexCoords).rgb;

    // тональная компрессия с экспозицией
    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
    // гамма-коррекция
    mapped = pow(mapped, vec3(1.0 / gamma));

    FragColor = vec4(mapped, 1.0);
}