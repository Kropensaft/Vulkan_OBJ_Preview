#version 450
// Input attribute from the vertex buffer, matching location 0 in the C++ vertex layout.
layout(location = 0) in vec3 inPosition;

// Input attribute from the vertex buffer, matching location 1 in the C++ vertex layout.
layout(location = 1) in vec3 inColor;

// The Uniform Buffer Object (UBO) from the C++ code.
// The 'binding = 0' MUST match the binding in your descriptor set layout.
// The member order ('view', then 'proj') MUST exactly match the order in your C++ struct.
layout(binding = 0) uniform UniformBufferObject {
    mat4 view;
    mat4 proj;
} ubo;

// Output variable that will be passed to the fragment shader.
// The 'location = 0' here is for communication between shader stages.
layout(location = 0) out vec3 fragColor;

void main() {
    // Calculate the final position of the vertex in clip space.
    // The order is critical: Projection * View * Model * Position.
    // Since we have no model matrix, it's just Proj * View * Pos.
    gl_Position = ubo.proj * ubo.view * vec4(inPosition, 1.0);

    // Pass the vertex's color directly to the fragment shader.
    // It will be automatically interpolated across the face of the triangle.
    fragColor = inColor;
}
