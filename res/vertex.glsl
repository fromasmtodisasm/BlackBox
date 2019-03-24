#version 330 core

layout (location = 0) in vec3 position;
uniform float offset;
uniform mat4x4 rotate;

out vec3 pos;

void main()
{
    gl_Position = rotate * vec4(position.x, position.y, position.z, 1.0);
    pos = gl_Position.xyz;
}
