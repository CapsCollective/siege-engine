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

layout(location=0) out vec3 cellColour;
layout(location=1) out flat float cellSpacing;
layout(location=2) out flat float lineWidth;
layout(location=3) out flat float cellMultiple;
layout(location=4) out flat float fadeFactor;
layout(location=5) out flat float scale;

layout (push_constant) uniform PushConstant {
    vec4 cellColouring;
    vec4 cellDimensions;
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