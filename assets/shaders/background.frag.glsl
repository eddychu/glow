#version 450 core
out vec4 FragColor;

layout(binding = 0) uniform samplerCube skybox;

in vec3 texCoords;

void main()
{    
    FragColor = texture(skybox, texCoords);
}