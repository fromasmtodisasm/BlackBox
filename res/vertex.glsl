#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 uv;
layout (location = 2) in vec3 normal;
uniform mat4x4 Model;
uniform mat4x4 View;
uniform mat4x4 Projection;

void main()
{
    gl_Position = Projection * View * Model * vec4(position.x, position.y, position.z, 1.0);
}
