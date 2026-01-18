#include "texture.h"

#include <iostream>
#include <string>
#include "glad/glad.h"
#include "stb_image.h"


bool textureFromFile(const std::string &path, const std::string &directory, unsigned int &textureId) {
    std::string filename;
    if (directory != "") {
        filename = std::format("{}/{}", directory, path);
    } else {
        filename = path;
    }

    std::print("Loading texture from file: '{}'\n", filename);

    int width, height, num_components;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &num_components, 0);

    if (!data) {
        stbi_image_free(data);
        std::print(std::cerr, "Texture failed to load at path: {}\n", filename);
        return false;
    }

    GLenum format;
    if (num_components == 1) {
        format = GL_RED;
    } else if (num_components == 2) {
        format = GL_RG;
    } else if (num_components == 3) {
        format = GL_RGB;
    } else if (num_components == 4) {
        format = GL_RGBA;
    } else {
        std::print(std::cerr, "Unknown format\n");
        stbi_image_free(data);
        return false;
    }

    glGenTextures(1, &textureId);

    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);

    return true;
}

bool textureFromFile(const std::string &path, unsigned int &textureId) {
    return textureFromFile(path, "", textureId);
}
