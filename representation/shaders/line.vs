#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 rgb;

uniform mat4 proj;

out vec3 vColor;

void main() {
    gl_Position = proj * vec4(pos, 1.0);
    vColor = rgb;
}