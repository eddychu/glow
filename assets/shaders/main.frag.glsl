#version 450 core
out vec4 FragColor;


layout (binding=0) uniform sampler2D shadow_map;

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
} vs_out;

void main()
{   
    vec3 light_dir = normalize(light.direction);
    vec3 view_dir = normalize(-vs_out.position);
    vec3 normal = normalize(vs_out.normal);
    float diff = max(dot(normal, light_dir), 0.0);
    vec3 diffuse = diff * light.color * light.intensity;
    // FragColor = vec4(diffuse, 1.0);
    float depth_value = texture(shadow_map, vs_out.uv).r;
    FragColor = vec4(vec3(depth_value), 1.0);
}