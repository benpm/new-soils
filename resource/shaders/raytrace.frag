#version 320 es
mediump out vec4 color;
precision mediump float;
precision mediump sampler3D;

in vec2 texCoord;
uniform sampler3D tex;
uniform float seconds;
uniform vec3 camPos;
uniform vec3 camRot;

int SIZE = 256;

float voxel(vec3 pos) {
    return texture(tex, pos / float(SIZE)).r;
}

float ray(vec3 start, vec3 direction) {
    direction = normalize(direction);
    for (int i = 0; i < 100; i++) {
        start += direction;
        if (voxel(start) > 0.99)
            return 1.0;
    }
    return 0.0;
}

void main() {
    float value;
    value = ray(camPos + vec3(texCoord, 0), camRot + vec3(texCoord.x - 0.5, texCoord.y - 0.5, 1.0));
    color = vec4(value, value, value, 1);
}
