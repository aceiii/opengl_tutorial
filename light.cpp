#include "light.h"

#include "fmt/core.h"
#include "fmt/ostream.h"

Lights::Lights(size_t initial_count)
    :_num_lights{initial_count}
{}

bool Lights::empty() const {
    return _num_lights == 0;
}

bool Lights::full() const {
    return _num_lights == MAX_LIGHTS;
}

size_t Lights::count() const {
    return _num_lights;
}

void Lights::addLight() {
    assert(_num_lights < MAX_LIGHTS);
    _num_lights += 1;
}

void Lights::removeLight() {
    int prevCount = _num_lights--;
    assert(_num_lights < prevCount);
}

Light& Lights::at(size_t index) {
    return _lights[index];
}

const Light& Lights::at(size_t index) const {
    return _lights[index];
}

void Lights::set(Shader shader) {
    for (size_t index = 0; index < _lights.size(); index += 1) {
        const Light &light = _lights[index];

        shader.setBool(fmt::format("lights[{}].enable", index), light.enable);
        shader.setInt(fmt::format("lights[{}].type", index), light.type);
        shader.setVec3(fmt::format("lights[{}].position", index), light.position);
        shader.setVec3(fmt::format("lights[{}].direction", index), light.direction);
        shader.setFloat(fmt::format("lights[{}].cutOff", index), light.cutoff);
        shader.setFloat(fmt::format("lights[{}].outerCutOff", index), light.outerCutoff);

        shader.setFloat(fmt::format("lights[{}].strength", index), light.strength);
        shader.setVec3(fmt::format("lights[{}].ambient", index), light.ambient);
        shader.setVec3(fmt::format("lights[{}].diffuse", index), light.diffuse);
        shader.setVec3(fmt::format("lights[{}].specular", index), light.specular);

        shader.setFloat(fmt::format("lights[{}].constant", index), light.constant);
        shader.setFloat(fmt::format("lights[{}].linear", index), light.linear);
        shader.setFloat(fmt::format("lights[{}].quadratic", index), light.quadratic);
    }

    shader.setInt("numLights", _num_lights);
}
