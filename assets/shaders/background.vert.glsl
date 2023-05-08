#version 450 core
layout (location = 0) in vec3 inPos;

uniform mat4 view;
uniform mat4 projection;

out vec3 texCoords;

void main()
{
    vec4 pos = projection * view * vec4(inPos, 1.0);
    gl_Position = pos.xyww;
    texCoords = inPos;
}