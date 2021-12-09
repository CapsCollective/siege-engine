#version 460

layout (location = 0) in vec3 fragColor;
layout (location = 1) in vec3 fragPosWorld;
layout (location = 2) in vec3 fragNormalWorld;

layout (location = 0) out vec4 outColor;

layout (set = 2, binding = 2) uniform LightPosition { // bindings are unique accross all shaders
    vec3 position;
} lightPosition;

const vec3 color = vec3(1.0, 1.0, 1.0);

void main() {
    vec3 directionToLight = lightPosition.position - fragPosWorld;
    float attenuation = 1.0 / dot(directionToLight, directionToLight); // distance squared

    vec3 lightColor = color * 0.2 * attenuation;
    vec3 ambientLight = vec3(1.0, 1.0, 1.0) * 0.02;
    vec3 diffuseLight = lightColor * max(dot(normalize(fragNormalWorld), normalize(directionToLight)), 0);
    outColor = vec4((diffuseLight + ambientLight) * fragColor, 1.0);
}