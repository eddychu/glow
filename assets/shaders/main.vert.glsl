#version 450 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT
{
    vec3 position;
    vec3 normal;
    vec2 uv;
} vs_out;

void main()
{
    vs_out.uv = inUV;
    vs_out.normal = transpose(inverse(mat3(view * model))) * inNormal;
    vs_out.position = (view * model * vec4(inPos, 1.0)).xyz;
    gl_Position = projection * view * model * vec4(inPos, 1.0);
}