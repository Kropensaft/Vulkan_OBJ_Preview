#version 450

layout(binding = 0) uniform Camera {
    mat4 viewMatrix;
    mat4 projectionMatrix;  // Add projection matrix
};

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec3 fragColor;

void main() {
    gl_Position = projectionMatrix * viewMatrix * vec4(inPosition, 1.0);
    fragColor = inColor;
}
