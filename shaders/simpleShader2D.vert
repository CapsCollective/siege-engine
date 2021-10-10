#version 460

// 'in' keyword specifies that position gets data from a buffer
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

layout(location = 0) out vec3 fragColor;

layout (std140, set = 0, binding = 0) readonly buffer Transforms{
    mat4 transforms[];
} transforms;

void main() {
    mat4 transform = transforms.transforms[gl_BaseInstance];

    gl_Position = transform * vec4(position, 1.0);
    fragColor = color;
}