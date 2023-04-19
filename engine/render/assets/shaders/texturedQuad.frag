#version 460

layout (location = 0) in vec3 fragColour;
layout (location = 1) in vec2 uv;
layout (location = 2) in flat uint texId;

layout (location = 0) out vec4 outColour;

layout(binding = 1) uniform sampler2D tex[16];

void main() {
    vec3 sampled = texture(tex[texId], uv).rgb;
    outColour = vec4(fragColour * sampled, 1.0);
}