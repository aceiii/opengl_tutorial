#ifndef __LIGHT_H__
#define __LIGHT_H__


struct Light {

    enum Type {
        POINT = 0,
        DIRECTIONAL,
        SPOT,
    };

    bool enable {true};

    Type type {POINT};
    glm::vec3 position {0.0f, 0.0f, 0.0f};
    glm::vec3 direction {0.0f, 0.0f, -1.0f};
    glm::vec3 ambient {0.164f, 0.24f, 0.4f};
    glm::vec3 diffuse {1.0f, 1.0f, 1.0f};
    glm::vec3 specular {0.5f, 0.7f, 0.9f};

    float strength {1.0f};
    float constant {1.0f};
    float linear {0.09f};
    float quadratic {0.032f};
    float cutoff {glm::cos(glm::radians(12.5f))};
    float outerCutoff {glm::cos(glm::radians(17.5f))};
};


#endif//__LIGHT_H__
