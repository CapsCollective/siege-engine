//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#version 460

layout (location = 0) in vec4 fragColour;
layout (location = 1) in vec2 uv;
layout (location = 2) in flat uint texId;

layout (location = 0) out vec4 outColour;

layout(binding = 1) uniform sampler2D tex[16];

void main() {
    // Glyphs are stored as black and white textures. The actual colour values are stored in the red channel to save
    // memory. We sample the red texture here.
    float distance = texture(tex[texId], uv).r;
    // We then compare the red channel of the current fragment with those of its surrounding fragments. The result is
    // the difference between all the neighbouring pixels.
    float smoothWidth = fwidth(distance);
    // finally, we compute the alpha channel based on the width provided above. We start at a value of 0.5 as a base so
    // that we have a smooth transition. This will interpolate the alpha based on the red channel so that all while pixels
    // become 0 and all black pixels become 1. Thus white pixels become transparent while black ones stay on the image
    float alpha = smoothstep(0.5 - smoothWidth, 0.5 + smoothWidth, distance);

    outColour = mix(vec4(fragColour.rgb, 0.0), fragColour, alpha);
}