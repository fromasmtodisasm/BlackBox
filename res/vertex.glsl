#version 330 core
layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 UV;
layout (location = 1) in vec3 VertexNormal;
out vec3 LightIntensity;
uniform vec3 LightPosition;
uniform vec3 Kd;
uniform vec3 Ld;
uniform vec3 color;
out vec3 FragPos;
out vec2 TextCoord;
out vec3 Normal;

uniform mat4 Model;
uniform mat4 View;
uniform mat3 NormalMatrix;
uniform mat4 Projection;
uniform mat4 MVP;

void main()
{
    gl_Position = Projection * View * Model * vec4(Position, 1.0f);
    FragPos = vec3(Model * vec4(Position, 1.0f));
    Normal = VertexNormal;
    TextCoord = UV;
}
