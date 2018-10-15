#ifndef __SHADER_H__
#define __SHADER_H__

#include <string>
#include <GLFW/glfw3.h>

class Shader
{
public:
    GLuint program;

    Shader();
    Shader(const char *vertexPath, const char *fragmentPath);

    bool init(const char *vertexPath, const char *fragmentPath);
    void use();

    static GLuint loadShaderFile(GLenum shaderType, const std::string &filename);
    static GLuint createShaderProgram(GLuint vertexShaderHandle, GLuint fragmentShaderHandle);
};

#endif//__SHADER_H__

