//
// Copyright (c) 2020-present Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
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
    /**
    * There are a few things happening here:
    * 1. The fract function returns the fraction of the given value (the parts after the decimal place). This returns to us
    * another normalised coordinate which we can then use to repeat patterns.
    * 2. We get the fraction of the uv (remember the has been divided by the spacing we provided earlier), and offset it
    * by half the screen.
    * 3. We then get the absolute value  of the new UV - half the screen (so that we don't get any negatives)
    * 4. We divide this by the derivative to give us a smaller space (used for anti-aliasing)
    * 5. We then use a step function to determine if the provided UV coordinate is within the cell borders
    */
    return max(step(abs(fract(uv - 0.5) -0.5) / derivative, width), factor);
}

vec4 grid(float spacing, float width, vec2 uv, vec3 lColour)
{
    float derivative = 1.0 / spacing;

    vec4 colour = vec4(getAxisGridColour(uv.y, width, getAxisGridColour(uv.x, width, 0.0, derivative), derivative));

    // Check if the uv sits within the range of the cell multiples
    vec2 modCoords = vec2(mod(uv.x, cellMultiple), mod(uv.y, cellMultiple));

    // Fade the cell colours depending on whether they fall within the cell multiples range or not
    vec4 mixColour = mix(vec4(cellColour * fadeFactor, fadeFactor), vec4(cellColour, 1.0), float(modCoords.x < width || modCoords.y < width));

    // Change the alpha based on whether the fragment is a grid line or not
    colour = mix(colour, vec4(mixColour), colour.a);

    return colour;
}

void main()
{
    // Determine the spacing and width of our cells
    const float spacing = cellSpacing * scale;
    const float width = lineWidth / spacing;

    // Determine the uv location of the fragment we're dealing with
    // NOTE: in glsl, gl_FragCoord always provides the fragment location + 0.5. As such, fragment 0,0 is represented as
    // (0.5,0.5)
    vec2 uv = (gl_FragCoord.xy - 0.5) / spacing;

    outColour = grid(spacing, width, uv, cellColour);
}