#version 460

layout (location = 0) in vec3 fragColour;
layout (location = 1) in vec2 uv;
layout (location = 2) in flat uint texId;

layout (location = 0) out vec4 outColour;

layout(binding = 1) uniform sampler2D tex[16];

void main() {
    vec4 sampled = vec4(texture(tex[texId], uv));
    outColour = vec4(fragColour, 1.0) * sampled;
}