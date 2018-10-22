#version 330 core

in vec2 TexCoord;

out vec4 color;

uniform sampler2D texture_diffuse0;

void main() {
    color = texture(texture_diffuse0, TexCoord);
}

