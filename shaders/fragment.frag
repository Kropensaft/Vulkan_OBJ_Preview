#version 450

// Input from Vertex Shader (must match location from vertex.vert)
layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec3 fragPos;
layout(location = 3) in vec4 fragPosLightSpace; // ⬅️ The new input

// Binding 2: Light UBO (needs light color/direction)
layout(binding = 2) uniform LightUbo {
    vec4 direction;
    vec4 color;
    mat4 lightSpaceMatrix;
} light;

// ⭐️ NEW: Binding 3 for Shadow Map ⭐️
layout(binding = 3) uniform sampler2D shadowMap; 

layout(location = 0) out vec4 outColor;

float calculateShadow(vec4 lightSpacePos) {
    // 1. Perspective divide (lightSpacePos is in clip space)
    vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;
    
    // 2. Transform to [0, 1] texture coordinates
    projCoords = projCoords * 0.5 + 0.5;
    
    // If we're outside the light frustum, assume full light
    if (projCoords.z > 1.0 || projCoords.x < 0.0 || projCoords.x > 1.0 || projCoords.y < 0.0 || projCoords.y > 1.0) {
      return 1.0;
    }

    // 3. Get the closest depth from the shadow map
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    
    // 4. Current fragment depth from the light's perspective
    float currentDepth = projCoords.z;

    // 5. Apply a bias to prevent self-shadowing artifacts (shadow "acne")
    float bias = 0.005; // You may need to tweak this
    
    // 6. Perform the comparison
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    
    return shadow; // Returns 1.0 if in shadow, 0.0 if lit
}

void main() {
    vec3 normal = normalize(fragNormal);
    vec3 lightDir = normalize(-light.direction.xyz);
    
    float shadowFactor = calculateShadow(fragPosLightSpace);
    
    // Ambient light (always applied)
    vec3 ambient = light.color.xyz * 0.1;
    
    // Diffuse light (only applied if NOT in shadow)
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.color.xyz * diff;
    
    // ⭐️ Apply shadow factor to the non-ambient lighting ⭐️
    diffuse = diffuse * (1.0 - shadowFactor); // (1.0 - 1.0) = 0 (shadowed), (1.0 - 0.0) = 1 (lit)

    vec3 finalColor = ambient + diffuse; // Add specular if you have it
    
    outColor = vec4(finalColor * fragColor, 1.0);
}
