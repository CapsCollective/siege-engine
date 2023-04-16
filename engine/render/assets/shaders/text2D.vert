#version 450

// Per instance data
layout(location = 0) in mat4 transform;
layout(location = 4) in vec4 texData;
layout(location = 5) in vec4 instanceColour;
layout(location = 6) in vec4 position;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec2 outUv;
layout(location = 2) out uint outTexId;

layout (push_constant) uniform Transform
{
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
    vec2 coords = vec2(texData.xy);
    vec2 dimensions = vec2(texData.zw);

    float uvx = (float(gl_VertexIndex == 0 || gl_VertexIndex == 1) * (texData.x + texData.z)) + (float(gl_VertexIndex == 2 || gl_VertexIndex == 3) * (texData.x));
    float uvy = (float(gl_VertexIndex == 0 || gl_VertexIndex == 3) * (texData.y + texData.w)) + (float(gl_VertexIndex == 1 || gl_VertexIndex == 2) * (texData.y));

    float posX = (float(gl_VertexIndex == 0 || gl_VertexIndex == 1) * (position.x + position.z)) + (float(gl_VertexIndex == 2 || gl_VertexIndex == 3) * position.x);
    float posY = (float(gl_VertexIndex == 0 || gl_VertexIndex == 3) * (position.y + position.w)) + (float(gl_VertexIndex == 1 || gl_VertexIndex == 2) * position.y);

    gl_Position = camera.projectionMatrix * camera.viewMatrix * transform * vec4(posX, posY, 0, 1.0);

    fragColor = instanceColour;
    outUv = vec2(uvx, uvy);
    outTexId = QuadTransform.texIndex;
}
