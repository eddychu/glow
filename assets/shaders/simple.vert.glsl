#version 450 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV;
layout (location = 3) in vec3 inTangent;
layout (location = 4) in vec3 inBitangent;

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
    vs_out.normal = transpose(inverse(mat3(model))) * inNormal;
    vs_out.position = (model * vec4(inPos, 1.0)).xyz;
    gl_Position = projection * view * model * vec4(inPos, 1.0);
}