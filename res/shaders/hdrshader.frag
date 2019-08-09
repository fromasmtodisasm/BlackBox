#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float alpha = 0.9;

void main()
{
    const float gamma = 2.2;
    vec3 hdrColor = texture(screenTexture, TexCoords).rgb;

    // тональная компрессия
    vec3 mapped = hdrColor / (hdrColor + vec3(1.0));
    // гамма-коррекция
    mapped = pow(mapped, vec3(1.0 / gamma));

    FragColor = vec4(mapped, 1.0) * vec4(1.0, 0.5, 0.5, 1.0);
}