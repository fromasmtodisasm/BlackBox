#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TextCoord;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D bumpTexture;

out vec4 color;

void main() {
  // Ambient
  float ambientStrength = 0.01f;
  vec3 ambient = ambientStrength * lightColor;

  // Diffuse
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(lightPos - FragPos);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * lightColor;

  vec4 result = vec4(ambient + diffuse,1.0);
  color = result * texture2D(diffuseTexture, TextCoord);

}
