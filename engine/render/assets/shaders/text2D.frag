#version 460

layout (location = 0) in vec4 fragColour;
layout (location = 1) in vec2 uv;
layout (location = 2) in flat uint texId;

layout (location = 0) out vec4 outColour;

layout(binding = 1) uniform sampler2D tex[16];

void main() {
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(tex[texId], uv).r);
    outColour = fragColour * sampled;
}