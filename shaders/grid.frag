#version 450

layout(location = 0) in vec3 fragColor;

layout(location = 1) in vec3 nearPoint;
layout(location = 2) in vec3 farPoint;
layout(location = 3) in mat4 fragView;
layout(location = 7) in mat4 fragProjection;

layout(location = 0) out vec4 outColor;

float near = 0.01; 
float far = 2;

vec2 vUV = vec2(4.0, 4.0);

vec4 grid(vec3 fragPos3D, float scale) { 
    vec2 coord = fragPos3D.xz * scale;
    vec2 derivative = fwidth(coord);
    vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;
    float line = min(grid.x, grid.y);
    float minimumZ = min(derivative.y, 1);
    float minimumX = min(derivative.x, 1);

    vec4 color = vec4(0.2, 0.2, 0.2, 1.0 - min(line, 1.0));

    color.z += (float(((fragPos3D.x > -1.0 * minimumX) && (fragPos3D.x < 1.0 * minimumX))) * 0.8);
    color.x += (float(((fragPos3D.z > -1.0 * minimumZ) && (fragPos3D.z < 1.0 * minimumZ))) * 0.8);

    return color;
}

float computeDepth(vec3 pos) {
    vec4 clipSpacePos = fragProjection * fragView * vec4(pos.xyz, 1.0);
    return (clipSpacePos.z / clipSpacePos.w);
}

float computeLinearDepth(vec3 pos) {
    vec4 clipSpacePos = fragProjection * fragView * vec4(pos.xyz, 1.0);
    float clipSpaceDepth = (clipSpacePos.z / clipSpacePos.w) * 2.0 - 1.0;
    float linearDepth = (2.0 * near * far) / (far + near - clipSpaceDepth * (far - near));
    return linearDepth / far;
}


void main() {

    float t = -nearPoint.y / (farPoint.y - nearPoint.y);

    vec3 fragPos3D = nearPoint + t * (farPoint - nearPoint);

    gl_FragDepth = computeDepth(fragPos3D);

    float linearDepth = computeLinearDepth(fragPos3D);
    float fading = max(0, (0.5 - linearDepth));
    
    outColor = (grid(fragPos3D, 1) + grid(fragPos3D, 1))* float(t > 0);
    outColor.a *= fading;
}