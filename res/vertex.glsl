#version 330 core

layout (location = 0) in vec3 position;
uniform float offset;
uniform mat4x4 Model;
uniform mat4x4 View;
uniform mat4x4 Projection;

out vec3 pos;

void main()
{
    gl_Position = Projection * View * Model * vec4(position.x, position.y, position.z, 1.0);
    pos = gl_Position.xyz;
}
