#version 330 core
#include "common.h"

#pragma attribute(position, aPos)
#pragma attribute(normal, aNormal)
#pragma attribute(tc, aTexCoords)

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
	vec3 oNormal;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lightSpaceMatrix;
uniform mat4 uvMatrix;
uniform int frame_id = 0;

void main()
{
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
    vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;
		vs_out.oNormal = aNormal;
    vs_out.TexCoords = (uvMatrix * vec4(aTexCoords, 1,1)).xy;
    vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
    //gl_Position = projection * view * model * vec4(aPos.x, sin(frame_id / 360.f) * aPos.y , aPos.z, 1.0);
    gl_Position = projection * view * model * vec4(aPos.x, aPos.y , aPos.z, 1.0);
}