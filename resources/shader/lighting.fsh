#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;

out vec4 color;

uniform vec3 viewPos;
uniform vec3 objectColor;
uniform vec3 lightColor;

uniform Material material;
uniform Light light;

void main() {

    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    float diff = max(dot(normal, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient = material.ambient * light.ambient;
    vec3 diffuse = (diff * material.diffuse) * light.diffuse;
    vec3 specular = (spec * material.specular) * light.specular;

    vec3 result = ambient + diffuse + specular;

    color = vec4(result, 1.0);
}

