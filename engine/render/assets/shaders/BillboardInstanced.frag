//
// Copyright (c) 2022 Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#version 450

layout (location = 0) in vec4 fragColour;
layout (location = 1) in vec2 uv;
layout (location = 2) in flat uint texId;

layout (location = 0) out vec4 outColour;

layout(binding = 1) uniform sampler2D tex[16];

void main() {
    vec4 sampled = texture(tex[texId], vec2(uv.x, -uv.y));
    outColour = sampled * fragColour;
}