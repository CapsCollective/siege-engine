//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#version 450

const vec3 OFFSETS[6] = vec3[](
    vec3(1, 1, 0), 
    vec3(-1, -1, 0), 
    vec3(-1, 1, 0),
    vec3(-1, -1, 0), 
    vec3(1, 1, 0), 
    vec3(1, -1, 0)
);

layout(location = 0) out vec3 outColor;

layout(location = 1) out vec3 nearPoint;
layout(location = 2) out vec3 farPoint;

layout(location = 3) out mat4 view;
layout(location = 7) out mat4 projection;

struct CameraData
{
    mat4 projectionMatrix;
    mat4 viewMatrix;
};

struct LightData
{
    vec4 lightColor;
    vec4 ambientLightColor;
    vec3 position;
};

layout (set = 0, binding = 0) uniform GlobalData {
    CameraData cameraData;
    LightData lightData;
} globalData;

CameraData camera = globalData.cameraData;

vec3 unprojectPoint(float x, float y, float z, mat4 view, mat4 projection) {
    mat4 viewInv = inverse(view);
    mat4 projInv = inverse(projection);
    vec4 unprojectedPoint = viewInv * projInv * vec4(x, y, z, 1.0);
    return unprojectedPoint.xyz / unprojectedPoint.w;
}

void main() {
    vec3 point = OFFSETS[gl_VertexIndex];

    nearPoint = unprojectPoint(point.x, point.y, 0.0, camera.viewMatrix, camera.projectionMatrix);
    farPoint = unprojectPoint(point.x, point.y, 1.0, camera.viewMatrix, camera.projectionMatrix);

    view = camera.viewMatrix;
    projection = camera.projectionMatrix;

    gl_Position = vec4(point, 1.0);
    outColor = vec3(1.0, 1.0, 1.0);
}

