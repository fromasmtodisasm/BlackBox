/*
#version 330 core
out vec4 color;

in vec3 Normal;
in vec3 FragPos;
in vec3 TextCoord;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform sampler2D diffuseTexture;

void main()
{
    // Ambient
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec4 result = vec4(ambient + diffuse,1.0);
    color = result * texture2D(diffuseTexture, TextCoord);
}
*/
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
#if 0
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(lightPos - FragPos);

  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * lightColor;

  vec4 result = vec4((diffuse) * lightColor, 1.0);
  color = texture2D(diffuseTexture, TextCoord)*result;
#endif

  // Ambient
  float ambientStrength = 0.1f;
  vec3 ambient = ambientStrength * lightColor;

  // Diffuse
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(lightPos - FragPos);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * lightColor;

  vec4 result = vec4(ambient + diffuse,1.0);
  color = result * texture2D(diffuseTexture, TextCoord);

}
