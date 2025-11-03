#version 450

// Input variable from the vertex shader.
// The 'location = 0' MUST match the 'out' location from the vertex shader.
layout(location = 0) in vec3 fragColor;

// The output variable that will be written to the framebuffer.
// The 'location = 0' here corresponds to the first (and only) color attachment in your render pass.
layout(location = 0) out vec4 outColor;

void main() {
    // Set the output color of the fragment.
    // We take the incoming vec3 color and add an alpha value of 1.0 to make it an opaque vec4.
    outColor = vec4(fragColor, 1.0);
}
