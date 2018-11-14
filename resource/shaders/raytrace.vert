#version 320 es

in vec4 VertexPosition, VertexColor;      
uniform float RadianAngle;
out vec4 TriangleColor;

void main() {
    gl_Position = VertexPosition;
}
