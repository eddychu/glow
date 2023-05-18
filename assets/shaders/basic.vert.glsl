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
    vec3 tangent;
    vec3 bitangent;
} vs_out;

void main()
{
    gl_Position = projection * view * model * vec4(inPos, 1.0);
    vs_out.position = vec3(model * vec4(inPos, 1.0));
    vs_out.normal = mat3(transpose(inverse(model))) * inNormal;
    vs_out.uv = inUV;
    vs_out.tangent = mat3(transpose(inverse(model))) * inTangent;
    vs_out.bitangent = mat3(transpose(inverse(model))) * inBitangent;
}