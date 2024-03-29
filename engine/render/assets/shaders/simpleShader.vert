//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#version 460

// 'in' keyword specifies that position gets data from a buffer
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 uv;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 fragPosWorld;
layout(location = 2) out vec3 fragNormalWorld;

struct ObjectData 
{
    mat4 transform;
    mat4 normalMatrix;
};

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

layout (std140, binding = 0) readonly buffer ObjectBuffer {
    ObjectData objects[];
} objectBuffer;

layout (binding = 1) uniform GlobalData {
    CameraData cameraData;
    LightData lightData;
} globalData;

void main() {

    ObjectData object = objectBuffer.objects[gl_InstanceIndex];

    vec4 positionWorld = object.transform * vec4(position, 1.0);

    CameraData camera = globalData.cameraData;

    gl_Position = camera.projectionMatrix * camera.viewMatrix * positionWorld;
    fragNormalWorld = normalize(mat3(object.normalMatrix) * normal);
    fragPosWorld = positionWorld.xyz;
    fragColor = color;
}