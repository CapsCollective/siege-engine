#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 colour;

layout (location = 0) out vec4 outColour;

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

struct BillboardData 
{
    vec3 position;
    vec3 scale;
};

layout (set = 0, binding = 0) uniform GlobalData {
    CameraData cameraData;
    LightData lightData;
} globalData;

layout (std140, set = 1, binding = 1) readonly buffer ObjectBuffer {
    BillboardData data[];
} objectBuffer;

mat4 view = globalData.cameraData.viewMatrix;
mat4 projection = globalData.cameraData.projectionMatrix;

void main() {

    int index = int(floor((int(gl_VertexIndex) / 4.0) - (int(gl_VertexIndex % 4 != 0) * 0.25)));

    BillboardData billboard = objectBuffer.data[index];

    // Find the light position in camera space
    vec4 lightInCameraSpace = view * vec4(billboard.position, 1.0);

    // Find the vertex position in camera space.
    vec4 positionInCameraSpace = lightInCameraSpace + vec4(position.xy * (billboard.scale.xy * 0.5), 0.0, 0.0);

    // Set the position to exist in camera space, not world space
    gl_Position = projection * positionInCameraSpace;

    outColour = colour;
}