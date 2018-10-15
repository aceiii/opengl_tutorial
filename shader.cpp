#include "shader.h"

#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader():program(0) {
}

Shader::Shader(const char *vertexPath, const char *fragmentPath):program(0) {
    init(vertexPath, fragmentPath);
}

void Shader::use() {
    glUseProgram(this->program);
}

bool Shader::init(const char *vertexPath, const char *fragmentPath) {
    GLuint vertex = Shader::loadShaderFile(GL_VERTEX_SHADER, vertexPath);
    GLuint fragment = Shader::loadShaderFile(GL_FRAGMENT_SHADER, fragmentPath);

    if (!vertex || !fragment) {
        return false;
    }

    program = Shader::createShaderProgram(vertex, fragment);

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return program != 0;
}

GLuint Shader::loadShaderFile(GLenum shaderType, const std::string &filename) {
    std::ifstream f(filename);
    if (!f.is_open()) {
        std::cerr << "Shader: Unable to open file: " << filename << std::endl;
        return 0;
    }

    std::string sourceFile((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
    const GLchar *shaderSource[] = { sourceFile.c_str() };

    GLuint handle = glCreateShader(shaderType);
    glShaderSource(handle, 1, shaderSource, nullptr);
    glCompileShader(handle);

    GLint success;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &success);

    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(handle, 512, NULL, infoLog);
        std::cerr << "Shader: Shader compilation failed: " << infoLog << std::endl;
        return 0;
    }

    return handle;
}

GLuint Shader::createShaderProgram(GLuint vertexShaderHandle, GLuint fragmentShaderHandle) {
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShaderHandle);
    glAttachShader(shaderProgram, fragmentShaderHandle);
    glLinkProgram(shaderProgram);

    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "Shader: Shader program link failed: " << infoLog << std::endl;
        return 0;
    }

    return shaderProgram;
}

