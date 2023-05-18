#version 450 core
out vec4 FragColor;
layout (binding = 0) uniform sampler2D texture_albedo;
in vec2 texCoord;

void main()
{
    FragColor = vec4(texCoord, 0.0, 1.0);
}