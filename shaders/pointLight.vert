#version 450

layout(location = 0) in vec2 position;

layout (location = 0) out vec2 fragOffset;

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

const float LIGHT_RADIUS = 0.05;

mat4 view = globalData.cameraData.viewMatrix;
mat4 projection = globalData.cameraData.projectionMatrix;

void main() {
    fragOffset = position;
    vec3 cameraRightWorld = {view[0][0], view[1][0], view[2][0]};
    vec3 cameraUpWorld = {view[0][1], view[1][1], view[2][1]};

    vec3 positionWorld = globalData.lightData.position.xyz
        + LIGHT_RADIUS * fragOffset.x * cameraRightWorld
        + LIGHT_RADIUS * fragOffset.y * cameraUpWorld;

    gl_Position = projection * view * vec4(positionWorld, 1.0);
}