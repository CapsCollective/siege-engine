#version 460

// 'in' keyword specifies that position gets data from a buffer
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

layout(location = 0) out vec3 fragColor;

layout (std140, set = 0, binding = 0) readonly buffer Transforms{
    mat4 transforms[];
} transforms;

struct CameraData
{
    mat4 projectionMatrix;
    mat4 viewMatrix;
};

layout (set = 1, binding = 1) uniform GlobalData {
    CameraData cameraData;
} globalData;

CameraData camera = globalData.cameraData;

void main() {
    mat4 transform = transforms.transforms[gl_BaseInstance];

    vec4 positionWorld = transform * vec4(position, 1.0);

    gl_Position = camera.projectionMatrix * camera.viewMatrix * positionWorld;
    
    fragColor = color;
}