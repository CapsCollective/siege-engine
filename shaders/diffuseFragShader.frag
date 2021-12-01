#version 460

layout (location = 0) in vec3 fragColor;
layout (location = 0) out vec4 outColor;

layout (set = 0, binding = 0) uniform LightDir {
    vec3 dir;
} lightDir;

void main() {
    outColor = vec4(fragColor, 1.0);
}