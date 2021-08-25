#version 460

// 'in' keyword specifies that position gets data from a buffer
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 uv;

layout(location = 0) out vec3 fragColor;

struct ObjectData 
{
    mat4 transform;
    mat4 normalMatrix;
};

layout (std140, set = 1, binding = 0) readonly buffer ObjectBuffer{
    ObjectData objects[];
} objectBuffer;

// Push constant for the model transform data
// layout (push_constant) uniform Push {
//     mat4 transform;
//     mat4 normalMatrix;
// } push;

// Uniform buffer for storing camera data
layout (set = 0, binding = 0) uniform CameraBuffer {
    mat4 projection;
    mat4 view;
    mat4 viewProj;
} cameraData;

const vec3 DIRECTION_TO_LIGHT = normalize(vec3(1.0, -3.0, -1.0));
const float AMBIENT = 0.03;

void main() {

    ObjectData object = objectBuffer.objects[gl_BaseInstance];
    mat4 transformMatrix = cameraData.viewProj * object.transform;

    gl_Position = transformMatrix * vec4(position, 1.0);

    vec3 normalWorldSpace = normalize(mat3(object.normalMatrix) * normal);

    float lightIntensity = AMBIENT + max(dot(normalWorldSpace, DIRECTION_TO_LIGHT), 0);

    fragColor = lightIntensity * color;
}