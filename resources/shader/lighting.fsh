#version 330 core

in vec3 FragPos;
in vec3 Normal;

out vec4 color;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

void main() {
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * objectColor;

    color = vec4(result, 1.0);
}

