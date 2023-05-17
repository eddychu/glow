#version 450 core
out vec4 FragColor;
layout (binding = 0) uniform sampler2D texture_albedo;
in vec2 TexCoord;

void main()
{
    FragColor = vec4(texture(texture_albedo, TexCoord).rgb, 1.0);
}