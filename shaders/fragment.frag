#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec3 fragPos;
layout(location = 3) in vec4 fragPosLightSpace; 

layout(set=0, binding = 2) uniform LightUbo {
    vec4 direction;
    vec4 color;
    mat4 lightSpaceMatrix;
} light;

layout(set = 1, binding = 0) uniform sampler2D shadowMap; 

layout(location = 0) out vec4 outColor;

float calculateShadow(vec4 lightSpacePos, float bias) {
    vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;
    projCoords.xy = projCoords.xy * 0.5 + 0.5;

    if (projCoords.z > 1.0 || projCoords.x < 0.0 || projCoords.x > 1.0 || projCoords.y < 0.0 || projCoords.y > 1.0) {
        return 0.0;
    }

    float currentDepth = projCoords.z;
    
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0; 

    return shadow;
}

void main() {

    vec3 normal = normalize(fragNormal);
    vec3 lightDir = normalize(-light.direction.xyz);

    float bias = 0.0;

    float shadowFactor = calculateShadow(fragPosLightSpace, bias);
    
    vec3 ambient = light.color.xyz * 0.1;
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.color.xyz * diff;
    
    float shadowIntensity = 0.5; 
    diffuse = diffuse * (1.0 - shadowFactor * shadowIntensity);
    vec3 finalColor = ambient + diffuse; 
    
    vec3 objectColor = vec3(0.4, 0.4, 0.4); // Dark Grey
    outColor = vec4(finalColor * objectColor, 1.0);
}
