#version 330 core
#include "texturedphong.inc"

out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

void main()
{           
     // obtain normal from normal map in range [0,1]
    vec3 normal = texture(normalMap, fs_in.TexCoords).rgb;
    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
   
    // get diffuse color
    vec4 color = texture(diffuseMap, fs_in.TexCoords).rgba;
	if (color.a < threshold)
		discard;
    // ambient
    vec3 ambient = ambient_factor * color.rgb;
    // diffuse
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color.rgb;
    FragColor = vec4(ambient + diffuse, 1.0);
    return;
    // specular
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 256);
    float distance = length(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float attenuation = 1;

    vec3 specular = vec3(1) * spec;
    FragColor = vec4(ambient + diffuse + specular, 1.0)/attenuation;
}