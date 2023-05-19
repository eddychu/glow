#version 450 core
out vec4 FragColor;
layout (binding = 0) uniform sampler2D texture_albedo;
layout (binding = 1) uniform sampler2D texture_normal;
layout (binding = 2) uniform sampler2D texture_metallic_roughness;
layout (binding = 3) uniform sampler2D texture_ao;
layout (binding = 4) uniform sampler2D texture_emissive;

uniform struct Light {
    vec3 position;
    vec3 color;    
    vec3 direction;
    float intensity;
    float cut_off;
    int type;
} lights[10];

uniform int light_count;
uniform vec3 camera_pos;

in VS_OUT
{
    vec3 position;
    vec3 normal;
    vec2 uv;
    vec3 tangent;
    vec3 bitangent;
} vs_out;

const float PI = 3.14159265359;

float distribution_ggx(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float geometry_schlick_ggx(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float nom = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float geometry_smith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = geometry_schlick_ggx(NdotV, roughness);
    float ggx1 = geometry_schlick_ggx(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnel_schlick_roughness(float cos_theta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cos_theta, 0.0, 1.0), 5.0);
}   

vec3 fresnel_schlick(float cos_theta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cos_theta, 0.0, 1.0), 5.0);
}

vec3 calc_bump_normal(vec3 normal, vec3 tangent, vec3 bitangent, vec3 normal_map)
{
    mat3 tbn = mat3(tangent, bitangent, normal);
    return normalize(tbn * normal_map);
}

vec3 calc_light_contrib(vec3 normal, vec3 view_dir, float roughness, float metallic, vec3 F0, vec3 albedo) {
    vec3 result = vec3(0.0);
    for (int i = 0; i < light_count; i++) {
        vec3 light_dir = vec3(0.0);
        float attenuation = 1.0;
        // directional light
        if (lights[i].type == 0) {
            light_dir = normalize(lights[i].position);
        } 
        // point light
        else if (lights[i].type == 1) {
            light_dir = normalize(lights[i].position - vs_out.position);
            float distance = length(lights[i].position - vs_out.position);
            attenuation = 1.0 / (distance * distance);
        } 
        // spot light
        else if (lights[i].type == 2) {
            light_dir = normalize(lights[i].position - vs_out.position);
            float angle = acos(dot(-light_dir, lights[i].direction));
            float cut_off = radians(clamp(lights[i].cut_off, 0.0, 90.0));
            if (angle < cut_off) {
                attenuation = pow(dot(-light_dir, lights[i].direction), 5.0);
            }
        }
        // float attenuation = 1.0 / (distance * distance);
        // float attenuation = 1.0;
        vec3 radiance = lights[i].color * lights[i].intensity * attenuation;
        vec3 half_dir = normalize(view_dir + light_dir);
        
        float ndf = distribution_ggx(normal, half_dir, roughness);
        float g = geometry_smith(normal, view_dir, light_dir, roughness);
        vec3 fresnel = fresnel_schlick(max(dot(half_dir, view_dir), 0.0), F0);

        vec3 numerator = ndf * g * fresnel;

        float denominator = 4.0 * max(dot(normal, view_dir), 0.0) * max(dot(normal, light_dir), 0.0) + 0.001;

        vec3 specular = numerator / denominator;

        vec3 kS = fresnel;
        vec3 kD = vec3(1.0) - kS;

        kD *= 1.0 - metallic;

        float n_dot_l = max(dot(normal, light_dir), 0.0);
        result += (kD * albedo / PI + specular) * radiance * n_dot_l;
    }
    return result;
}

vec3 srgb_to_linear(vec3 color) {
    return pow(color, vec3(2.2));
}

vec3 linear_to_srgb(vec3 color) {
    return pow(color, vec3(1.0 / 2.2));
}

void main()
{

    vec4 metal_roughness = texture(texture_metallic_roughness, vs_out.uv);
    vec3 emmisive = texture(texture_emissive, vs_out.uv).rgb;
    float ao = texture(texture_ao, vs_out.uv).r;
    float metallic = metal_roughness.b;
    float roughness = metal_roughness.g;
    vec3 albedo = srgb_to_linear(texture(texture_albedo, vs_out.uv).rgb);
    vec3 normal_map = texture(texture_normal, vs_out.uv).xyz;
    normal_map = normal_map * 2.0 - 1.0;
    vec3 normal = calc_bump_normal(normalize(vs_out.normal), vs_out.tangent, vs_out.bitangent, normal_map);
    vec3 view_dir = normalize(camera_pos - vs_out.position);

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);



    vec3 result = calc_light_contrib(normal, view_dir, roughness, metallic, F0, albedo);

    vec3 ambient = vec3(0.1) * albedo * ao;

    vec3 color = ambient + result + emmisive;

    color = linear_to_srgb(color);

    FragColor = vec4(color, 1.0);
}