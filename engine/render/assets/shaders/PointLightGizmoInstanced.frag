//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#version 450

layout (location = 0) in vec2 fragOffset;
layout (location = 1) in vec4 lightColour;

layout (location = 0) out vec4 outColor;

void main() {
    float dis = sqrt(dot(fragOffset, fragOffset));

    if (dis >= 1.0) discard;
    
    outColor = vec4(lightColour);
}