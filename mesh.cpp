#include "mesh.h"

#include "glad/glad.h"
#include "fmt/core.h"
#include "fmt/ostream.h"

Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, std::vector<Texture> &textures):_vertices{vertices}, _indices{indices}, _textures{textures} {
    fmt::print("Mesh::Mesh()\n");
    fmt::print("  num vertices: {}\n", _vertices.size());
    fmt::print("  num indices: {}\n", _indices.size());
    fmt::print("  num textures: {}\n", _textures.size());

    setupMesh();
}

void Mesh::draw(Shader shader) {

    unsigned int diffuseIndex = 0;
    unsigned int specularIndex = 0;

    for (unsigned int i = 0; i < _textures.size(); i += 1) {
        const Texture &texture = _textures[i];

        glActiveTexture(GL_TEXTURE0 + i);

        int number;
        std::string name = texture.type;

        if (name == "texture_diffuse") {
            number = diffuseIndex++;
        } else if (name == "texture_specular") {
            number = specularIndex++;
        }

        std::string attributeName = fmt::format("{}{}", name, number);

        shader.setInt(attributeName, i);
        glBindTexture(GL_TEXTURE_2D, texture.id);
    }

    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}

std::vector<Vertex>& Mesh::getVertices() {
    return _vertices;
}

const std::vector<Vertex>& Mesh::getVertices() const {
    return _vertices;
}

std::vector<unsigned int>& Mesh::getIndices() {
    return _indices;
}

const std::vector<unsigned int>& Mesh::getIndices() const {
    return _indices;
}

std::vector<Texture>& Mesh::getTextures() {
    return _textures;
}

const std::vector<Texture>& Mesh::getTextures() const {
    return _textures;
}

void Mesh::setupMesh() {
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_ebo);

    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), &_vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), &_indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    glBindVertexArray(0);
}
