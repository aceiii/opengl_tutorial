#version 330 core

in vec3 vertexColor;
in vec2 TexCoord;

out vec4 color;

uniform sampler2D ourTexture;

void main() {
    color = texture(ourTexture1, TexCoord) * vec4(vertexColor, 1.0f);
}

