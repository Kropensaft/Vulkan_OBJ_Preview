#version 450

// --------------------------- INPUTS ---------------------------
// Input from Vertex Buffer (assuming this structure)
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec2 inTexCoord; 

// -------------------------- OUTPUTS ---------------------------
// Output to Fragment Shader (must match fragment.frag locations)
layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 fragNormal;
layout(location = 2) out vec3 fragPos;          // ⭐️ FIX 4: Declares fragPos (Location 2)
layout(location = 3) out vec4 fragPosLightSpace; 

// -------------------------- UBOs ------------------------------
// Binding 0: Camera UBO (view/proj/model)
layout(binding = 0) uniform CameraUBO {
    mat4 view;
    mat4 proj;
    mat4 model;
} ubo;

layout(binding = 1) uniform NormalMatrixUBO {
    mat4 normal; 
} normal_ubo;

// Binding 2: Light UBO
layout(binding = 2) uniform LightUbo {
    vec4 direction;
    vec4 color;
    mat4 lightSpaceMatrix; 
} light;

void main() {
    mat4 model = ubo.model;
    
    vec4 worldPos = model * vec4(inPosition, 1.0);
    
    gl_Position = ubo.proj * ubo.view * worldPos;

    fragColor = inColor;
    
    fragNormal = mat3(normal_ubo.normal) * inNormal; 
    
    fragPos = worldPos.xyz; 
    
    fragPosLightSpace = light.lightSpaceMatrix * worldPos; 
}
