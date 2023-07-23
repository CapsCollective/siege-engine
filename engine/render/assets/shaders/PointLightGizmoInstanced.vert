#version 450

// Per instance data
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec4 inColour;

layout (location = 0) out vec2 fragOffset;
layout (location = 1) out vec4 lightColour;

struct CameraData
{
    mat4 projectionMatrix;
    mat4 viewMatrix;
};

vec3 quadVertices[4] = {
    vec3(1.0, 1.0, 0),
    vec3(1.0, -1.0, 0),
    vec3(-1.0, -1.0, 0),
    vec3(-1.0, 1.0, 0)
};

layout (set = 0, binding = 0) uniform GlobalData {
    CameraData cameraData;
} globalData;

const float LIGHT_RADIUS = 0.05;

mat4 view = globalData.cameraData.viewMatrix;
mat4 projection = globalData.cameraData.projectionMatrix;

void main() {
    fragOffset = quadVertices[gl_VertexIndex].xy;

    // Find the light position in camera space
    vec4 lightInCameraSpace = view * vec4(inPosition, 1.0);

    // Find the vertex position in camera space.
    vec4 positionInCameraSpace = lightInCameraSpace + LIGHT_RADIUS * vec4(fragOffset, 0.0, 0.0);

    // Set the position to exist in camera space, not world space
    gl_Position = projection * positionInCameraSpace;
    lightColour = inColour;
}