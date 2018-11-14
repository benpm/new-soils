#version 320 core
mediump out vec4 color;
precision mediump float;

mediump uniform samplerBuffer voxels;
const int CHUNK_SIZE = 32;

uniform mat4 viewProj;
/* 
uint vox_at(vec3 pos) {
    int x = int(mod(pos.x, float(CHUNK_SIZE)));
    int y = int(mod(pos.y, float(CHUNK_SIZE)));
    int z = int(mod(pos.z, float(CHUNK_SIZE)));

    return texelFetch(voxels, (x * CHUNK_SIZE + y) * CHUNK_SIZE + z).x;
}

float ray(vec3 start, vec3 end) {
    float dist = 64.0;
    float i;
    for (i = 0.0; i < 1.0; i += 0.01) {
        vec3 pos = mix(start, end, i);
        if (vox_at(pos) > uint(0)) {
            dist = distance(start, pos);
            break;
        }
    }

    return dist;
}
 */
void main() {

    /* float value = ray(
        vec3(gl_FragCoord.x / 8.0, gl_FragCoord.y / 8.0, 0.0), 
        vec3(gl_FragCoord.x / 8.0, gl_FragCoord.y / 8.0, 32.0)) / 64.0; */
    color = texelFetch(voxels, 0);
}
