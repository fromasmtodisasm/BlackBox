#version 450 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;


interface vs_out 
{
	vec3 get_color();	
};

struct vs_out_impl : vs_out 
{
	vec3 get_color()
	{

		return vec3(1, 0,0);
	}
};

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
    vec3 color = texture(diffuseMap, fs_in.TexCoords).rgb;
	//vec3 color = vs_out_impl::get_color(); // Intresting possibility
	vec3 emissive = vec3(0.0f);
	if (has_emissive)
	{
		emissive = texture(emissiveMap, fs_in.TexCoords).rgb;
	}
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightColor = vec3(1.1);
	if (!lightOn)
	{
		FragColor = vec4(color, 1.0f);
		return;
	}

    // ambient
    vec3 ambient = 0.15 * color; 
    // diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    float spec = 0.0;

    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;
	if (has_specular)
	{


		specular *= vec3(texture(specularMap, fs_in.TexCoords));
		//FragColor = vec4(10, 0, 0, 1.0);
		//specular *= vec3(10, 0, 0);

	}
	specular + vec3(10, 0, 0);
    // calculate shadow
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);

    vec3 lighting =(ambient + (1.0 - shadow) * (diffuse + specular)) * color + emissive * emissive_factor;

	vec3 result = lighting;
	FragColor = vec4(result, alpha);
}