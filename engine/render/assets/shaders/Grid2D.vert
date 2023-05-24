//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#version 450

vec2 quadVertices[4] = {
    vec2(1.0, 1.0),
    vec2(1.0, -1.0),
    vec2(-1.0, -1.0),
    vec2(-1.0, 1.0)
};

layout(location=0) out vec3 lineColour;
layout(location=1) out vec2 cellSize;
layout(location=2) out vec2 resolution;
layout(location=3) out flat float lineThickness;

layout (push_constant) uniform PushConstant {
    vec4 lineColourAndThickness;
    vec4 cellSizeAndResolution;
} pushConstant;

void main()
{
    vec2 pos = quadVertices[gl_VertexIndex];
    gl_Position = vec4(quadVertices[gl_VertexIndex], 0.0, 1.0);

    lineColour = pushConstant.lineColourAndThickness.xyz;
    cellSize = pushConstant.cellSizeAndResolution.xy;
    resolution = pushConstant.cellSizeAndResolution.zw;
    lineThickness = pushConstant.lineColourAndThickness.w;
}