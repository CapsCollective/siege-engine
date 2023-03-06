#version 450

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec4 inColor;
layout (location = 2) in vec2 inUv;
layout(location = 3) in uint texId;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 outUv;
layout(location = 2) out uint outTexId;

layout (std140, binding = 0) readonly buffer Transforms{
    mat4 transforms[];
} transforms;

struct CameraData
{
    mat4 projectionMatrix;
    mat4 viewMatrix;
};

layout (binding = 1) uniform GlobalData {
    CameraData cameraData;
} globalData;

CameraData camera = globalData.cameraData;

void main() {
    int index = int(floor((int(gl_VertexIndex) / 4.0) - (int(gl_VertexIndex % 4 != 0) * 0.25)));

    mat4 transform = transforms.transforms[index];

    vec4 positionWorld = transform * vec4(inPosition, 0.0, 1.0);

    gl_Position = camera.projectionMatrix * camera.viewMatrix * positionWorld;

    fragColor = vec3(inColor);
    outUv = inUv;
    outTexId = texId;
}