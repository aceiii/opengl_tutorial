#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emissive;
    float shininess;
};

struct Light {
    bool isDirectional;
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float strength;
    float constant;
    float linear;
    float quadratic;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 color;

uniform vec3 viewPos;
uniform vec3 objectColor;
uniform vec3 lightColor;

uniform Material material;
uniform Light light;

void main() {
    vec3 lightDir;
    float attenuation = 1.0;

    if (light.isDirectional) {
        lightDir = normalize(-light.position);
    } else {
        float distance = length(light.position - FragPos);
        attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
        lightDir = normalize(light.position - FragPos);
    }

    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    float diff = max(dot(normal, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient = light.strength * light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.strength * light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.strength * light.specular * spec * vec3(texture(material.specular, TexCoords));
    vec3 emissive = vec3(texture(material.emissive, TexCoords));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 result = ambient + diffuse + specular + emissive;

    color = vec4(result, 1.0);
}

