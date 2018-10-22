#version 330 core

#define MAX_LIGHTS 8

struct Light {
    bool enable;

    int type;
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float strength;
    float constant;
    float linear;
    float quadratic;

    float cutOff;
    float outerCutOff;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 color;

uniform vec3 viewPos;

uniform sampler2D texture_diffuse0;
uniform sampler2D texture_specular0;

uniform int numLights;
uniform Light[MAX_LIGHTS] lights;

vec3 calculateLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    if (!light.enable) {
        return vec3(0.0);
    }

    float shininess = 32.0;

    vec3 lightDir;
    float attenuation = 1.0;
    float intensity = 1.0;

    if (light.type == 0) {
        float distance = length(light.position - fragPos);
        attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
        lightDir = normalize(light.position - fragPos);
    } else if (light.type == 1) {
        lightDir = normalize(-light.direction);
    } else {
        float distance = length(light.position - fragPos);
        attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
        lightDir = normalize(light.position - fragPos);
        float theta = dot(lightDir, normalize(-light.direction));
        float epsilon = light.cutOff - light.outerCutOff;
        intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    }

    vec3 reflectDir = reflect(-lightDir, normal);

    float diff = max(dot(normal, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

    vec3 ambient = light.strength * light.ambient * texture(texture_diffuse0, TexCoords).rgb;
    vec3 diffuse = light.strength * light.diffuse * diff * texture(texture_diffuse0, TexCoords).rgb;
    vec3 specular = light.strength * light.specular * spec * texture(texture_specular0, TexCoords).rgb;

    ambient *= attenuation;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return ambient + diffuse + specular;
}

void main() {
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = vec3(0.0);

    for (int i = 0; i < numLights; i += 1) {
        result += calculateLight(lights[i], normal, FragPos, viewDir);
    }

    color = vec4(result, 1.0);
}

