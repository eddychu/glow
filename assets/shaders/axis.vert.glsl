#version 450 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV;
layout (location = 3) in vec3 inTangent;
layout (location = 4) in vec3 inBitangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 AxisColor;

void main()
{
    gl_Position = projection * view * model * vec4(inPos, 1.0);
    AxisColor = inNormal;
}