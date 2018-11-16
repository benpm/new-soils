#version 320 es
mediump out vec4 color;
precision mediump float;
precision mediump sampler3D;

in vec2 texCoord;
uniform sampler3D tex;
uniform float seconds;

void main() {
    color = vec4(texture(tex, vec3(texCoord, seconds)).r, 0, 0, 1);
}
