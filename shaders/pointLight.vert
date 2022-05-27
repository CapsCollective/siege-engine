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

    // Find the light position in camera space
    vec4 lightInCameraSpace = view * vec4(globalData.lightData.position, 1.0);

    // Find the vertex position in camera space.
    vec4 positionInCameraSpace = lightInCameraSpace + LIGHT_RADIUS * vec4(fragOffset, 0.0, 0.0);

    // Set the position to exist in camera space, not world space
    gl_Position = projection * positionInCameraSpace;
}