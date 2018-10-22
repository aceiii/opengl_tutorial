#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"

class Model {
public:
    Model() = delete;
    explicit Model(const std::string &path);
    void draw(Shader shader);
    bool loaded() const;

private:
    bool loadModel(const std::string &path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *material, aiTextureType type, const std::string &typeName);

private:
    bool _loaded {false};
    std::vector<Mesh> _meshes;
    std::string _directory;
};


#endif//__MODEL_H__
