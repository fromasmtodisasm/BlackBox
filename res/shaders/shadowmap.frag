#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;


in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
	vec3 oNormal;
} fs_in;

struct FogInfo {
	float maxDist;
	float minDist;
	vec3 color;
};
struct Light {
    // vec3 position; //Ќе требуетс€ дл€ направленного источника.
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform FogInfo Fog;

uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
uniform sampler2D emissiveMap;
uniform sampler2D shadowMap;

uniform float emissive_factor = 20.0f;
uniform bool has_emissive = false;
uniform bool has_specular = false;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform bool lightOn = true;
uniform bool bloomOn = true;
uniform float bloomThreshold = 0.0f;
uniform bool isTerrain = false;
uniform bool shadowOn = false;
uniform float alpha = 1.0;

uniform Light dirLight;

float ShadowCalculation(vec4 fragPosLightSpace)
{
	if (!shadowOn)
		return 0.f;	
   // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
	if (projCoords.z > 1.0)
		return 0.0;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
	float bias = 0.005;
	float shadow = 0.0f;//currentDepth - bias > closestDepth  ? 1.0 : 0.0;
	vec2 texelSize = 1.0f / textureSize(shadowMap,0);
	int factor = 4;
	for (int x = -1*factor; x <= 1*factor; x++)
	{
		for (int y = -1*factor; y <= 1*factor; y++)
		{
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x,y)*texelSize).r;
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
		}
	}
	shadow /= 9.0 * factor * factor;
    return shadow; 
}

void main()
{
#define SH 64
    vec3 color = texture(diffuseMap, fs_in.TexCoords).rgb;
	vec3 emissive = vec3(0.0f);
	if (has_emissive)
	{
		emissive = texture(emissiveMap, fs_in.TexCoords).rgb;
	}
    vec3 normal = normalize(fs_in.Normal);

    // ambient
    vec3 ambient = dirLight.ambient*color; 
    // diffuse
    vec3 lightDir = normalize(-dirLight.direction);
    float diff = max(dot(normal, lightDir), 0.0) + 0.2;
    vec3 diffuse = diff * dirLight.diffuse * color;
    // specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), SH);
    vec3 specular = dirLight.specular * spec;// * texture(SH, fs_in.TexCoords).rgb;  
	vec3 mat_spec = vec3(0.1);
	if (has_specular)
	{
		mat_spec = texture(specularMap, fs_in.TexCoords).rgb;
	}
	specular *= mat_spec;
        
    vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, alpha);
}
