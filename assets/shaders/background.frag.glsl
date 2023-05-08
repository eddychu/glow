#version 450 core
out vec4 FragColor;

in vec3 texCoords;

void main()
{    
    FragColor = vec4(texCoords, 1.0);
}