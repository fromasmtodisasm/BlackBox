#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
    bool hasNormal;
    float shininess;
}; 

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

#define NR_POINT_LIGHTS 4

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;
uniform int countOfPointLights;

// function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec2 TexCoords);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 TexCoords);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 TexCoords);

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
    mat3 TBN;
} fs_in;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

void main()
{           
     // obtain normal from normal map in range [0,1]
    vec3 normal = texture(normalMap, fs_in.TexCoords).rgb;
    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);

    /*
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 1024);
    //float distance = length(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float attenuation =  + 0.032*distance + 0.005*distance*distance;

    vec3 specular = vec3(1) * spec;
    */

        // == =====================================================
    // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
    // For each phase, a calculate function is defined that calculates the corresponding color
    // per lamp. In the main() function we take all the calculated colors and sum them up for
    // this fragment's final color.
    // == =====================================================
    // phase 1: directional lighting
    vec3 result = CalcDirLight(dirLight, normal, viewDir, fs_in.TexCoords);
    //vec3 result;
    // phase 2: point lights
    for(int i = 0; i < countOfPointLights; i++)
        result += CalcPointLight(pointLights[i], normal, fs_in.TangentFragPos, viewDir, fs_in.TexCoords);
    // phase 3: spot light
    result += CalcSpotLight(spotLight, normal, fs_in.TangentFragPos, viewDir, fs_in.TexCoords);
    
    FragColor = vec4(result, 1.0);
}

// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec2 TexCoords)
{

  vec3 lightDir = normalize(-light.direction);
  // diffuse shading
  float diff = max(dot(normal, lightDir), 0.0);
  // specular shading
  vec3 reflectDir = reflect(-lightDir, normal);
  vec3 halfwayDir = normalize(lightDir + viewDir);  
  float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
  // combine results
  vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
  vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
  vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
  return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 TexCoords)
{
  vec3 lightDir = fs_in.TBN * normalize(light.position - fs_in.FragPos);
  // diffuse shading
  float diff = max(dot(normal, lightDir), 0.0);
  // specular shading
  vec3 reflectDir = reflect(-lightDir, normal);
  vec3 halfwayDir = normalize(lightDir + viewDir);  
  float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

  // attenuation
  float distance = length(light.position - fragPos);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
  // combine results
  vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
  vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
  vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
  ambient *= attenuation;
  diffuse *= attenuation;
  specular *= attenuation;
  return (ambient + diffuse + specular);
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 TexCoords)
{
  vec3 lightDir = fs_in.TBN * normalize(light.position - fs_in.FragPos);
  // diffuse shading
  float diff = max(dot(normal, lightDir), 0.0);
  // specular shading
  vec3 reflectDir = reflect(-lightDir, normal);
  vec3 halfwayDir = normalize(lightDir + viewDir);  
  float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

  // attenuation
  float distance = length(light.position - fragPos);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
  // spotlight intensity
  float theta = dot(lightDir, fs_in.TBN * normalize(-light.direction)); 
  float epsilon = light.cutOff - light.outerCutOff;
  float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
  // combine results
  vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
  vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
  vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
  ambient *= attenuation * intensity;
  diffuse *= attenuation * intensity;
  specular *= attenuation * intensity;
  return (ambient + diffuse + specular);
}