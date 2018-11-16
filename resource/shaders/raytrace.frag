#version 320 es
mediump out vec4 color;
precision mediump float;
precision mediump sampler2D;

in vec2 texCoord;
uniform sampler2D tex;

void main() {
    color = vec4(texture(tex, texCoord).rgb, 1);
}
