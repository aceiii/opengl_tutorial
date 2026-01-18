#include "shader.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <print>


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
        std::print(std::cerr, "Shader: Unable to open file: {}\n", filename);
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
        std::print(std::cerr, "Shader: Shader compilation failed: {}\n", infoLog);
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
        std::print(std::cerr, "Shader: Shader program link failed: {}\n", infoLog);
        return 0;
    }

    return shaderProgram;
}

void Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(program, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(program, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(program, name.c_str()), value);
}

void Shader::setVec2(const std::string &name, const glm::vec2 &value) const {
    glUniform2fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);
}

void Shader::setVec2(const std::string &name, float x, float y) const {
    glUniform2f(glGetUniformLocation(program, name.c_str()), x, y);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const {
    glUniform3fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const std::string &name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(program, name.c_str()), x, y, z);
}

void Shader::setVec4(const std::string &name, const glm::vec4 &value) const {
    glUniform4fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);
}

void Shader::setVec4(const std::string &name, float x, float y, float z, float w) const {
    glUniform4f(glGetUniformLocation(program, name.c_str()), x, y, z, w);
}

void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const {
    glUniformMatrix2fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const {
    glUniformMatrix3fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

