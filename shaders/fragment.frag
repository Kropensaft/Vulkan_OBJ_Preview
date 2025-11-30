#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragNormal;

layout(location = 0) out vec4 outColor;

layout(binding = 0) uniform UniformBufferObject {
    mat4 view;
    mat4 proj;
    mat4 model;
} ubo;

layout(binding = 2) uniform LightUbo {
    vec4 direction;
    vec4 color;
} light;

void main() {
    vec3 N = normalize(fragNormal);

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * light.color.rgb;

    vec3 lightDir = normalize(-light.direction.xyz);
    
    float diff = max(dot(N, lightDir), 0.0);
    vec3 diffuse = diff * light.color.rgb;

    vec3 result = (ambient + diffuse) * fragColor;
    outColor = vec4(result, 1.0);
}
