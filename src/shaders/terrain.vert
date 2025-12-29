#version 330 core
layout (location = 0) in vec3 aPos;

// Uniforms
uniform mat4 u_MVP;

void main() {
    // position * camera matrix
    gl_Position = u_MVP * vec4(aPos, 1.0);
}