#version 150

#ifdef GL_ES
precision mediump float;
#endif

out vec4 out_FragColor;
in vec2 fragTexCoord;
in vec3 fragNormal;
uniform sampler2D texSampler;

void main() {
    if (fragNormal.x == 100000000) {
        return;
    }
    out_FragColor = texture(texSampler, fragTexCoord);
}