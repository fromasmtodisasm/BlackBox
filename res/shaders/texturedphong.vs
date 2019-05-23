#version 330 core
layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 UV;
layout (location = 2) in vec3 VertexNormal;
layout (location = 3) in vec3 Tangent;
layout (location = 4) in vec3 Btangent;

out vec3 FragPos;
out vec2 TexCoords;
out mat3 TBN;

uniform mat4 Model;
uniform mat4 View;
uniform mat3 NormalMatrix;
uniform mat4 Projection;
uniform mat4 MVP;

void main()
{
    gl_Position = Projection * View * Model * vec4(Position, 1.0f);
    FragPos = vec3(Model * vec4(Position, 1.0f));
    vec3 T = NormalMatrix * Tangent;
    vec3 B = NormalMatrix * Btangent;
    vec3 N = NormalMatrix * VertexNormal;
    TBN = mat3(T,B,N);
    TexCoords = UV;
}
