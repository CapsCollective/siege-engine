//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#version 460

layout(location=0) in vec3 cellColour;
layout(location=1) in flat float cellSpacing;
layout(location=2) in flat float lineWidth;
layout(location=3) in flat float cellMultiple;
layout(location=4) in flat float fadeFactor;
layout(location=5) in flat float scale;

// output variables
layout(location = 0) out vec4 outColour;

float getAxisGridColour(float uv, float width, float factor, float derivative)
{
    return max(step(abs(fract(uv - 0.5) -0.5) / derivative, width), factor);
}

vec4 grid(float spacing, float width, vec2 uv, vec3 lColour)
{
    float derivative = 1.0 / spacing;

    vec4 colour = vec4(getAxisGridColour(uv.y, width, getAxisGridColour(uv.x, width, 0.0, derivative), derivative));

    vec2 modCoords = vec2(mod(uv.x, cellMultiple), mod(uv.y, cellMultiple));

    vec4 mixColour = mix(vec4(cellColour * fadeFactor, fadeFactor), vec4(cellColour, 1.0), float(modCoords.x < width || modCoords.y < width));

    colour = mix(colour, vec4(mixColour), colour.a);

    return colour;
}

void main()
{
    const float spacing = cellSpacing * scale;
    const float width = lineWidth / spacing;

    vec2 uv = (gl_FragCoord.xy - 0.5) / spacing;

    outColour = grid(spacing, width, uv, cellColour);
}