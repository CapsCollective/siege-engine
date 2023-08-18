//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#version 450

vec2 quadVertices[4] = {
    vec2(1.0, 1.0),
    vec2(1.0, -1.0),
    vec2(-1.0, -1.0),
    vec2(-1.0, 1.0)
};

layout(location=0) out vec3 cellColour; // The colour of our grid lines
layout(location=1) out flat float cellSpacing; // the spacing between lines in pixels
layout(location=2) out flat float lineWidth; // the width of our lines
layout(location=3) out flat float cellMultiple; // determines how many cells will be encompassed in a sub-grid
layout(location=4) out flat float fadeFactor; // determines how much sub-cells are faded
layout(location=5) out flat float scale; // the DPI scale of the screen

layout (push_constant) uniform PushConstant {
    vec4 cellColouring; // Stores the cellColour in xyz and fadeFactor in w
    vec4 cellDimensions; // Stores the cell spacing in x, multiple in y, dimensions in z, and width in w
} pushConstant;

void main()
{
    vec2 pos = quadVertices[gl_VertexIndex];
    gl_Position = vec4(quadVertices[gl_VertexIndex], 0.0, 1.0);

    cellColour = pushConstant.cellColouring.xyz;
    fadeFactor = pushConstant.cellColouring.w;
    cellSpacing = pushConstant.cellDimensions.x;
    cellMultiple = pushConstant.cellDimensions.y;
    scale = pushConstant.cellDimensions.z;
    lineWidth = pushConstant.cellDimensions.w;
}