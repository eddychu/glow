#version 450 core
out vec4 FragColor;


in vec3 AxisColor;

void main()
{
    FragColor = vec4(AxisColor, 1.0f);
}