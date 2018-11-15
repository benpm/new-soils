#version 320 es

in vec4 VertexPosition;      
out vec2 texCoord;

void main() {
    texCoord = VertexPosition.xy;
    gl_Position = VertexPosition;
}
