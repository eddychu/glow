#version 450 core
out vec4 FragColor;

uniform vec3 camera_pos;

layout (binding = 0) uniform sampler2D texture_albedo;
layout (binding = 1) uniform sampler2D texture_normal;
// layout (binding = 2) uniform sampler2D texture_metallic_roughness;
// layout (binding = 3) uniform sampler2D texture_ao;
// layout (binding = 4) uniform sampler2D texture_emissive;


uniform struct Light {
    vec3 direction;
    vec3 color;    
    float intensity;
} lights[3];

in VS_OUT
{
    vec3 position;
    vec3 normal;
    vec2 uv;
    vec3 tangent;
    vec3 bitangent;
} vs_out;



vec3 calc_bump_normal(vec3 normal, vec3 tangent, vec3 bitangent, vec3 normal_map)
{
    mat3 tbn = mat3(tangent, bitangent, normal);
    return normalize(tbn * normal_map);
}

vec3 calc_light_contrib(vec3 normal, vec3 view_dir) {
    vec3 result = vec3(0.0);
    for (int i = 0; i < 3; i++) {
        vec3 light_dir = normalize(lights[i].direction);
        vec3 half_dir = normalize(view_dir + light_dir);
        vec3 diffuse = max(dot(normal, light_dir), 0.0) * lights[i].color;
        vec3 specular = pow(max(dot(half_dir, normal), 0.0), 32.0) * lights[i].color;
        vec3 ambient = vec3(0.1, 0.1, 0.1);
        result += (diffuse + ambient + specular) * lights[i].intensity;
    }
    return result;
}

void main()
{   
    vec3 normal_map = texture(texture_normal, vs_out.uv).xyz;
    normal_map = normal_map * 2.0 - 1.0;
    vec3 normal = calc_bump_normal(normalize(vs_out.normal), vs_out.tangent, vs_out.bitangent, normal_map);

    vec3 view_dir = normalize(camera_pos - vs_out.position);
    // hard code light
    vec3 result = calc_light_contrib(normal, view_dir);
    vec3 albedo = texture(texture_albedo, vs_out.uv).rgb;
    FragColor = vec4(result * albedo, 1.0);
}