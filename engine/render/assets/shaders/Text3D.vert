//
// Copyright (c) 2020-present Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#version 450

// Per instance data
layout(location = 0) in mat4 transform;
layout(location = 4) in vec4 texData; // Stores the minimum x and y values for the glyph in the texture + the glyph's dimensions in the texture
layout(location = 5) in vec4 colour;
layout(location = 6) in vec4 coordinates; // stores x and y coordinates in space + the glyph's dimensions in space

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec2 outUv;
layout(location = 2) out uint outTexId;

layout (push_constant) uniform TextureData
{
    uint textureIndex;
} textureData;

struct CameraData
{
    mat4 projectionMatrix;
    mat4 viewMatrix;
};

layout (binding = 0) uniform GlobalData {
    CameraData cameraData;
} globalData;

CameraData camera = globalData.cameraData;

void main() {
    vec2 coords = vec2(texData.xy);
    vec2 dimensions = vec2(texData.zw);

    // Find the UV based on the vertex index + min values = coordinate + dimension
    float uvx = (float(gl_VertexIndex == 0 || gl_VertexIndex == 1) * (texData.x + texData.z))
        + (float(gl_VertexIndex == 2 || gl_VertexIndex == 3) * (texData.x));
    float uvy = (float(gl_VertexIndex == 0 || gl_VertexIndex == 3) * (texData.y + texData.w))
        + (float(gl_VertexIndex == 1 || gl_VertexIndex == 2) * (texData.y));

    // Find the vertex position based off the vertex index + min values = coordinate + dimension
    float posX = (float(gl_VertexIndex == 0 || gl_VertexIndex == 1) * (coordinates.x + coordinates.z))
        + (float(gl_VertexIndex == 2 || gl_VertexIndex == 3) * coordinates.x);
    float posY = (float(gl_VertexIndex == 0 || gl_VertexIndex == 3) * (coordinates.y + coordinates.w))
        + (float(gl_VertexIndex == 1 || gl_VertexIndex == 2) * coordinates.y);

    gl_Position = camera.projectionMatrix * camera.viewMatrix * transform * vec4(posX, posY, 0, 1.0);

    fragColor = colour;
    outUv = vec2(uvx, uvy);
    outTexId = textureData.textureIndex;
}
