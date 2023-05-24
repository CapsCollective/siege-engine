//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#version 460

layout(location=0) in vec3 lineColour;
layout(location=1) in vec2 cellSize;
layout(location=2) in vec2 resolution;
layout(location=3) in flat float lineThickness;

// output variables
layout(location = 0) out vec4 outColour;

const float scale = 2.0;
const float lineSpacing = 100.0;
const float lineWidth = 1.0;
const float darkenMultiple = 2.0;
const float fadeFactor = 0.5;

vec4 grid(float spacing, float width, vec2 uv, vec3 lColour)
{
    vec2 derivative = fwidth(uv);

    float grid = 0.0;
    grid = max(step(abs(fract(uv.x - 0.5) -0.5) / derivative.x, width), grid);
    grid = max(step(abs(fract(uv.y - 0.5) -0.5) / derivative.y, width), grid);

    vec4 colour = vec4(grid);

    bool isSolid = colour.x != 0.0 && colour.y != 0.0 && colour.z != 0.0;

    float mx = mod(uv.x, float(darkenMultiple));
    float my = mod(uv.y, float(darkenMultiple));

    bool isMultiple = mx < width || my < width;

    vec4 fadedColour = vec4(lColour * fadeFactor, fadeFactor);

    vec4 mixColour = vec4((float(isMultiple) * vec4(lineColour, 1.0)) + (float(!isMultiple) * fadedColour));

    colour = mix(colour, vec4(mixColour), colour.a);

    return colour;
}

void main()
{
    float spacing = lineSpacing * scale; // i.e: 40.0
    float width = lineWidth / spacing; // i.e: 0.025

    vec2 uv = gl_FragCoord.xy - 0.5;

    uv.x *= 1.0 / spacing;
    uv.y /= spacing;

    outColour = grid(spacing, width, uv, lineColour);
}