#ifndef __MESH_H__
#define __MESH_H__

#include <vector>
#include <string>
#include <glm/glm.hpp>

#include "shader.h"


struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh {
public:
    Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, std::vector<Texture> &textures);
    void draw(Shader shader);

    std::vector<Vertex>& getVertices();
    const std::vector<Vertex>& getVertices() const;
    std::vector<unsigned int>& getIndices();
    const std::vector<unsigned int>& getIndices() const;
    std::vector<Texture>& getTextures();
    const std::vector<Texture>& getTextures() const;

private:
    void setupMesh();

    unsigned int _vbo, _vao, _ebo;

    std::vector<Vertex> _vertices;
    std::vector<unsigned int> _indices;
    std::vector<Texture> _textures;
};


#endif//__MESH_H__
