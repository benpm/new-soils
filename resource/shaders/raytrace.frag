#version 320 es
mediump out vec4 color;
precision mediump float;
precision mediump sampler3D;

in vec2 texCoord;
uniform sampler3D tex;
uniform float seconds;
uniform vec3 camPos;
uniform vec3 camRot;
uniform float imageAspectRatio;
uniform vec2 screenSize;
uniform int volSize;

float focal;

float voxel(vec3 pos) {
    return texture(tex, pos / float(volSize)).r;
}

float ray(vec3 start, vec3 direction) {
    float accum = 0.0;
    vec3 rayPos = start;
    vec3 voxelPos;
    for (int i = 0; i < 100; i++) {
        rayPos += direction;
        if (voxel(rayPos) > 0.0) {
            return distance(rayPos, start);
        }
    }
    return 100.0;
}

void main() {
    float focal = 60.0;
    float angle = tan(focal * 0.5 * 3.1415926535897932384626433832795 / 180.0); // convert from degree to radian
    float xx = (2.0 * (gl_FragCoord.x / screenSize.x) - 1.0) * angle;
    float yy = (1.0 - 2.0 * (gl_FragCoord.y / screenSize.y)) * angle;
    float value;
    vec3 dir = vec3(
        cos(camRot.y + xx) * cos(camRot.x + yy),
        sin(camRot.y + xx) * cos(camRot.x + yy),
        sin(camRot.x + yy));
    value = ray(camPos, dir) / 100.0;
    color = vec4(value, value, value, 1);

    
}
