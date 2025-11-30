#version 450

layout(location = 0) in vec3 inPosition;

layout(binding = 0) uniform UniformBufferObject {
    mat4 view;
    mat4 proj;
    mat4 model;
} ubo;

layout(binding = 2) uniform LightUbo {
    vec4 direction;
    vec4 color;
    mat4 lightSpaceMatrix; 
} light;

void main() {
    gl_Position = light.lightSpaceMatrix * ubo.model * vec4(inPosition, 1.0);
}
