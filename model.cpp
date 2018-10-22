#include "model.h"

#include <iostream>

#include "glad/glad.h"
#include "fmt/core.h"
#include "fmt/ostream.h"
#include "texture.h"

namespace {
    std::vector<Texture> textures_loaded;
}

Model::Model(const std::string &path) {
    _loaded = loadModel(path);
}

bool Model::loaded() const {
    return _loaded;
}

void Model::draw(Shader shader) {
    for (auto &mesh : _meshes) {
        mesh.draw(shader);
    }
}

bool Model::loadModel(const std::string &path) {
    fmt::print("Mesh::loadModel('{}')\n", path);

    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        fmt::print(std::cerr, "ERROR::ASSIMP::{}", importer.GetErrorString());
        return false;
    }

    _directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);

    fmt::print("Model loaded: {} meshes\n", _meshes.size());

    return true;
}

void Model::processNode(aiNode *node, const aiScene *scene) {
    fmt::print("Mesh::processNode()\n");
    for (unsigned int i = 0; i < node->mNumMeshes; i += 1) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        _meshes.push_back(processMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i += 1) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    fmt::print("Mesh::processMesh()\n");
    fmt::print("  num vertices: {}\n", mesh->mNumVertices);
    fmt::print("  num faces: {}\n", mesh->mNumFaces);

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i += 1) {
        Vertex vertex;

        vertex.position = glm::vec3 {
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z,
        };

        vertex.normal = glm::vec3 {
            mesh->mNormals[i].x,
            mesh->mNormals[i].y,
            mesh->mNormals[i].z,
        };

        if (mesh->mTextureCoords[0]) {
            vertex.texCoords = glm::vec2 {
                mesh->mTextureCoords[0][i].x,
                mesh->mTextureCoords[0][i].y,
            };
        } else {
            vertex.texCoords = glm::vec2 {0.0, 0.0};
        }

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i += 1) {
        aiFace face = mesh->mFaces[i];

        for (unsigned int j = 0; j < face.mNumIndices; j += 1) {
            indices.push_back(face.mIndices[j]);
        }
    }

    if (mesh->mMaterialIndex >= 0) {
        fmt::print("  material index: {}\n", mesh->mMaterialIndex);

        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(end(textures), begin(diffuseMaps), end(diffuseMaps));

        std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(end(textures), begin(specularMaps), end(specularMaps));
    }

    return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *material, aiTextureType type, const std::string &typeName) {
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < material->GetTextureCount(type); i += 1) {
        aiString string;
        material->GetTexture(type, i, &string);

        bool skip = false;

        for (unsigned int j = 0; j < textures_loaded.size(); j += 1) {
            if (std::strcmp(textures_loaded[j].path.c_str(), string.C_Str()) == 0) {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }

        if (!skip) {
            unsigned int textureId;
            if (!textureFromFile(string.C_Str(), _directory, textureId)) {
                fmt::print(std::cerr, "Failed to load texture: {}/{}\n", _directory, string.C_Str());
            }


            Texture texture;
            texture.id = textureId;
            texture.type = typeName;
            texture.path = string.C_Str();

            textures.push_back(texture);
            textures_loaded.push_back(texture);
        }
    }

    return textures;
}

