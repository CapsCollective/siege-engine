#version 460

layout (location = 0) in vec3 fragColor;
layout (location = 1) in vec3 fragPosWorld;
layout (location = 2) in vec3 fragNormalWorld;

layout (location = 0) out vec4 outColor;

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

layout (set = 1, binding = 1) uniform GlobalData {
    CameraData cameraData;
    LightData lightData;
} globalData;

LightData lightData = globalData.lightData;

void main() {
    vec3 directionToLight = lightData.position - fragPosWorld;
    float attenuation = 1.0 / dot(directionToLight, directionToLight); // distance squared

    vec3 lightColor = lightData.lightColor.xyz * lightData.lightColor.w * attenuation;
    vec3 ambientLight = lightData.ambientLightColor.xyz * lightData.ambientLightColor.w;
    vec3 diffuseLight = lightColor * max(dot(normalize(fragNormalWorld), normalize(directionToLight)), 0);
    outColor = vec4((diffuseLight + ambientLight) * fragColor, 1.0);
}