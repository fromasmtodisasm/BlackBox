#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;
uniform mat4 projection = mat4(1.0);
uniform mat4 model = mat4(1.0);
uniform mat3 uv_transform = mat3(1.0);

void main()
{
    gl_Position = projection * model * vec4(aPos.x, aPos.y, 1.0, 1.0); 
    TexCoords = (uv_transform * vec3(aTexCoords, 1.0)).xy;
} 