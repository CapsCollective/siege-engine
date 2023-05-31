//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#version 460

layout (location = 0) in vec4 fragColour;
layout (location = 1) in vec2 uv;
layout (location = 2) in flat uint texId;

layout (location = 0) out vec4 outColour;

layout(binding = 1) uniform sampler2D tex[16];

void main() {
    float distance = texture(tex[texId], uv).r;
    float smoothWidth = fwidth(distance);
    float alpha = smoothstep(0.5 - smoothWidth, 0.5 + smoothWidth, distance);

    outColour = mix(vec4(fragColour.rgb, 0.0), fragColour, alpha);
}