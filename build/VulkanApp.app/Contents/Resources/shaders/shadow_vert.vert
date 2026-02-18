#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec2 inTexCoord;

layout(set = 0, binding = 0) uniform CameraUBO {
    mat4 view;
    mat4 proj;
    mat4 model;
} ubo;

layout(set = 0, binding = 2) uniform LightUbo {
    vec4 direction;
    vec4 color;
    mat4 lightSpaceMatrix; 
} light;

void main() {
    vec4 worldPos = ubo.model * vec4(inPosition, 1.0);
    
    gl_Position = light.lightSpaceMatrix * worldPos;
}
