#version 330 core
layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 UV;
layout (location = 2) in vec3 VertexNormal;
out vec4 FragPos;
out vec2 TexCoords;
out vec3 Normal;

uniform mat4 Model;
uniform mat4 View;
uniform mat3 NormalMatrix;
uniform mat4 Projection;
uniform mat4 MVP;

void main()
{
    gl_Position = Projection * View * Model * vec4(Position, 1.0f);
    FragPos = vec4(Model * vec4(Position, 1.0f));
    Normal = NormalMatrix * VertexNormal;
    TexCoords = UV;
}
