#version 450 core
out vec4 FragColor;

in VS_OUT
{
    vec3 normal;
    vec2 uv;
} vs_out;

void main()
{    
    FragColor = vec4(vs_out.uv, 0.0, 1.0);
}