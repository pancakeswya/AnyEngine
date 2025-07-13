#version 150 core

#ifdef GL_ES
precision mediump float;
#endif

out vec4 out_FragColor;
in vec2 fragTexCoord;
uniform sampler2D texSampler;

void main() {
    out_FragColor = texture(texSampler, fragTexCoord);
}