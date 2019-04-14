#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TextCoord;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform sampler2D ourTexture;

out vec4 color;

layout( location = 0 ) out vec4 FragColor;
void main() {
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(lightPos - FragPos);

  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * lightColor;

  vec3 result = (0.3 + diffuse) * vec3(0.8, 0, 0.4);
  //color = vec4(result, 1.0f);
  color = texture(ourTexture, TextCoord);//*vec4(0.7, 0.3, 0.9, 1);

}
