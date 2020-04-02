#version 330 core

layout(triangles) in;
layout(line_strip, max_vertices = 4) out;

in vec3 normal[];

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
	vec3 oNormal;
} gs_in[];

out GS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
	vec3 oNormal;
} gs_out;


out vec3 vertexNormal;

void main() {
	for(int i = 0; i < 4; i++) {
		gl_Position = gl_in[i % 3].gl_Position;

		gs_out.FragPos = gs_in[i % 3].FragPos;
		gs_out.FragPos = gs_in[i % 3].FragPos;
		gs_out.Normal = gs_in[i % 3].Normal;
		gs_out.TexCoords = gs_in[i % 3].TexCoords;
		gs_out.FragPosLightSpace = gs_in[i % 3].FragPosLightSpace;
		gs_out.oNormal = gs_in[i % 3].oNormal;

		//vertexNormal = normal[i % 3];
		EmitVertex();
	}
	EndPrimitive();
}