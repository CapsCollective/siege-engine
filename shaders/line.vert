#version 450

layout(location = 0) in vec3 position;

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
    vec3 color;
} lineData;

CameraData camera = globalData.cameraData;

void main() {
    vec4 positionInWorldSpace = camera.projectionMatrix * camera.viewMatrix * vec4(position, 1.0);
    gl_Position = positionInWorldSpace;
    outColor = lineData.color;
}