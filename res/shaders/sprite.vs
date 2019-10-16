#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 uv_projection;

void main()
{
    gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = (uv_projection * vec4(vertex.z, vertex.w, 0.f, 1.0)).xy;
}  
