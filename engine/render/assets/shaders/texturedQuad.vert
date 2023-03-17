#version 450

layout(location = 0) in vec2 inPosition;
layout (location = 1) in vec2 inUv;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 outUv;
layout(location = 2) out uint outTexId;

layout (push_constant) uniform Transform
{
    mat4 transform;
    vec4 texColour;
    uint texIndex;
} QuadTransform;

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
    vec4 positionWorld = QuadTransform.transform * vec4(inPosition, 0.0, 1.0);

    gl_Position = camera.projectionMatrix * camera.viewMatrix * positionWorld;

    fragColor = vec3(QuadTransform.texColour);
    outUv = inUv;
    outTexId = QuadTransform.texIndex;
}