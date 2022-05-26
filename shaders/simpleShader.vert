#version 460

// 'in' keyword specifies that position gets data from a buffer
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 uv;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 fragPosWorld;
layout(location = 2) out vec3 fragNormalWorld;

struct ObjectData 
{
    mat4 transform;
    mat4 normalMatrix;
};

layout (std140, set = 0, binding = 0) readonly buffer ObjectBuffer{
    ObjectData objects[];
} objectBuffer;

layout (set = 1, binding = 1) uniform ProjectionView {
    mat4 projectionView;
} projectionView;

void main() {

    ObjectData object = objectBuffer.objects[gl_BaseInstance];

    vec4 positionWorld = object.transform * vec4(position, 1.0);

    gl_Position = projectionView.projectionView * positionWorld;
    fragNormalWorld = normalize(mat3(object.normalMatrix) * normal);
    fragPosWorld = positionWorld.xyz;
    fragColor = color;
}