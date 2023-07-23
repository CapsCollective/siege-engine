#version 450

layout (location = 0) in vec2 fragOffset;
layout (location = 1) in vec4 lightColour;

layout (location = 0) out vec4 outColor;

void main() {
    float dis = sqrt(dot(fragOffset, fragOffset));

    if (dis >= 1.0) discard;
    
    outColor = vec4(lightColour);
}