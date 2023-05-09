#version 450 core
out vec4 FragColor;


layout (binding=0) uniform sampler2D shadow_map;

uniform vec3 view_pos;

uniform struct Light {
    vec3 direction;
    vec3 color;    
    float intensity;
} light;


in VS_OUT
{
    vec3 position;
    vec3 normal;
    vec2 uv;
    vec4 position_light_space;
} vs_out;

float linearize_depth(float depth)
{
    float near_plane = 0.1;
    float far_plane = 100.0;
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));	
}

float shadow_caculate(vec4 frag_pos_light_space, float bias)
{
    vec3 proj_coords = frag_pos_light_space.xyz / frag_pos_light_space.w;
    proj_coords = proj_coords * 0.5 + 0.5;
    float closest_depth = texture(shadow_map, proj_coords.xy).r;
    float current_depth = proj_coords.z;
    float shadow = 0.0;
    vec2 texel_size = 1.0 / textureSize(shadow_map, 0);
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            float pcf_depth = texture(shadow_map, proj_coords.xy + vec2(x, y) * texel_size).r; 
            shadow += current_depth - bias > pcf_depth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    return shadow;
}

void main()
{   
    vec3 light_dir = normalize(light.direction);
    vec3 view_dir = normalize(view_pos - vs_out.position);
    vec3 normal = normalize(vs_out.normal);
    float diff = max(dot(normal, light_dir), 0.0);
    float spec = pow(max(dot(normal, normalize(light_dir + view_dir)), 0.0), 32.0);
    float attenuation = 1.0 / (1.0 + 0.09 * length(vs_out.position - light.direction));
    vec3 diffuse = diff * light.color * light.intensity * attenuation;
    vec3 specular = spec * light.color * light.intensity * attenuation;
    float bias = max(0.05 * (1.0 - dot(normal, light_dir)), 0.005);
    float shadow = shadow_caculate(vs_out.position_light_space , bias);
    vec3 ambient = vec3(0.1, 0.1, 0.1) * light.color * light.intensity;
    FragColor = vec4(ambient + (1.0 - shadow) * (diffuse + specular), 1.0);
}