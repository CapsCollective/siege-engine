#version 460

// 'in' keyword specifies that position gets data from a buffer
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

layout(location = 0) out vec3 fragColor;

struct ObjectData 
{
    mat4 transform;
    mat4 normalMatrix;
};

layout (std140, set = 0, binding = 0) readonly buffer ObjectBuffer{
    ObjectData objects[];
} objectBuffer;

void main() {
    ObjectData object = objectBuffer.objects[gl_BaseInstance];
    mat4 transformMatrix = object.transform;

    gl_Position = transformMatrix * vec4(position, 1.0);
    fragColor = color;
}