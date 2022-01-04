#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

layout(location = 0) out vec3 outColor;

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

layout (set = 1, binding = 1) uniform LineData
{
    vec3 origin;
    vec3 destination;
    vec3 color;
} lineData;

vec3 positions[2] = vec3[](
    lineData.origin,
    lineData.destination
);

CameraData camera = globalData.cameraData;

void main() {
    vec4 positionInWorldSpace = camera.projectionMatrix * camera.viewMatrix * vec4(position + positions[gl_VertexIndex], 1.0);
    gl_Position = positionInWorldSpace;
    outColor = lineData.color;
}