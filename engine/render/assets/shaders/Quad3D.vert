//
// Copyright (c) 2020-present Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#version 450

// Per instance data
layout (location = 0) in mat4 inTransform;
layout (location = 4) in vec4 inColour;
layout (location = 5) in vec4 inUv;

layout (location = 0) out vec4 outColour;
layout (location = 1) out vec2 outUv;
layout (location = 2) out flat uint outTexId;

layout (push_constant) uniform PushConstant {
    uint textureIndex;
} pushConstant;

vec3 quadVertices[4] = {
    vec3(1.0, 1.0, 0),
    vec3(1.0, -1.0, 0),
    vec3(-1.0, -1.0, 0),
    vec3(-1.0, 1.0, 0)
};

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
    gl_Position = camera.projectionMatrix * camera.viewMatrix * inTransform * vec4(quadVertices[gl_VertexIndex], 1.0);

    float uvx = (float(gl_VertexIndex == 0 || gl_VertexIndex == 1) * (inUv.x + inUv.z))
    + (float(gl_VertexIndex == 2 || gl_VertexIndex == 3) * (inUv.x));
    float uvy = (float(gl_VertexIndex == 0 || gl_VertexIndex == 3) * (inUv.y + inUv.w))
    + (float(gl_VertexIndex == 1 || gl_VertexIndex == 2) * (inUv.y));

    outColour = inColour;
    outUv = vec2(uvx, uvy);
    outTexId = pushConstant.textureIndex;
}