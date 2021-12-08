#version 460

layout (location = 0) in vec3 fragColor;
layout (location = 1) in vec3 fragPosWorld;
layout (location = 2) in vec3 fragNormalWorld;

layout (location = 0) out vec4 outColor;

layout (set = 2, binding = 2) uniform LightPosition { // bindings are unique accross all shaders
    vec3 position;
} lightPosition;

const vec3 color = vec3(0.0, 0.0, 0.0);

void main() {

    vec3 dirToLight = lightPosition.position - fragPosWorld;
    float attenuation = 1.0 / dot(dirToLight, dirToLight);

    vec3 lightColor = color * 0.2 * attenuation;
    vec3 ambientLight = vec3(0.0, 1.0, 0.0) * 0.1;
    vec3 diffuseLight = lightColor * max(dot(normalize(fragNormalWorld), normalize(dirToLight)), 0);

    outColor = vec4((diffuseLight + ambientLight) * fragColor, 1.0);
}