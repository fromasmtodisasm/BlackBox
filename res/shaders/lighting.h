#include "tonemap/unreal.glsl"
struct Light
{
	vec3 Position;
	//vec3
};
float diffuse(vec3 lightPos, vec3 fragPos, vec3 N)
{
    vec3 lightDir = normalize(lightPos - fragPos);
    return max(dot(normalize(N), lightDir), 0.0);
}

float specular(vec3 lightPos, vec3 fragPos, vec3 eyePos, vec3 N)
{
	vec3 reflectedLightVector = normalize(reflect(-(lightPos - fragPos), normalize(N)));
	vec3 eyeVector			  = normalize(eyePos - fragPos);
	float s					  = max(dot(eyeVector, reflectedLightVector), 0.0);
	return pow(s, 128);
}

vec3 ambient()
{
	return vec3(0.05, 0.05, 0.05);
}

vec3 wo_tonemap(vec3 x)
{
	return x;
}

#ifdef TM_UNREAL
#define tonemap unreal
#else
#define tonemap wo_tonemap
#endif
