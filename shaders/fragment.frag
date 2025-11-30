#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragNormal;

layout(location = 0) out vec4 outColor;

// Binding 0: Still the MVP matrices (though not used in fragment shader, it's part of the set)
layout(binding = 0) uniform UniformBufferObject {
    mat4 view;
    mat4 proj;
    mat4 model;
} ubo;

// Binding 1: Our new directional light UBO
layout(binding = 2) uniform LightUbo {
    vec4 direction;
    vec4 color;
} light;

void main() {
    // --- Basic Lighting Calculation ---
    // A small amount of light to ensure the object is never fully black
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * light.color.rgb;

    // Calculate the diffuse intensity
    // We negate the light's direction vector because we need the direction FROM the surface TO the light
    vec3 lightDir = normalize(-light.direction.xyz);
    float diff = max(dot(fragNormal, lightDir), 0.0);
    vec3 diffuse = diff * light.color.rgb;

    // Combine the light components and multiply by the object's base color
    vec3 result = (ambient + diffuse) * fragColor;
    outColor = vec4(result, 1.0);
}
