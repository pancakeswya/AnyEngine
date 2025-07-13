#version 150 core

in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;

out vec2 fragTexCoord;
out vec3 fragNormal;

layout(std140) uniform ubo {
    mat4 model;
    mat4 view;
    mat4 proj;
};

void main() {
    gl_Position = proj * view * model * vec4(inPosition, 1.0);
    fragTexCoord = inTexCoord;
    fragNormal = inNormal;
}