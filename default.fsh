#version 330 core

in vec3 vertexColor;
in vec2 TexCoord;

out vec4 color;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

void main() {
    vec2 uv = vec2(TexCoord.x, -TexCoord.y);
    color = mix(texture(ourTexture1, uv), texture(ourTexture2, uv), 0.2);
}

