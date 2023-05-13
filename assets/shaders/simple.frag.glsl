#version 450 core
out vec4 FragColor;

layout (binding = 0) uniform sampler2D texture_albedo;
// layout (binding = 1) uniform sampler2D texture_normal;
// layout (binding = 2) uniform sampler2D texture_metallic_roughness;
// layout (binding = 3) uniform sampler2D texture_ao;
// layout (binding = 4) uniform sampler2D texture_emissive;

in VS_OUT
{
    vec3 position;
    vec3 normal;
    vec2 uv;
} vs_out;



void main()
{   
    // vec3 normal = texture(texture_normal, vs_out.uv).xyz;
    // vec3 position = vs_out.position;
    // vec3 v_normal = normalize(vs_out.normal);
    vec3 albedo = texture(texture_albedo, vs_out.uv).rgb;
    FragColor = vec4(albedo, 1.0);
}