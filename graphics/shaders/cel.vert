#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 fragPos;
out vec3 fragNorm;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
    gl_Position = projection * view * model * vec4(position, 1.0);
    fragPos = vec3(model * vec4(position, 1.0f));
    fragNorm = normal;
}

