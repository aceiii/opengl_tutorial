#include <iostream>
#include <fstream>
#include <string>
#include <complex>

#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

namespace {
    GLFWwindow *window = nullptr;

    GLfloat vertices[] = {
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
    };
    GLuint indices[] = {
        0, 1, 2
    };

    GLuint vboHandle, vaoHandle, eboHandle;
    GLuint shaderProgram;
    GLuint vertexColorLocation;
}

GLuint loadShaderFile(GLenum shaderType, const std::string &filename) {
    std::ifstream f(filename);
    if (!f.is_open()) {
        std::cerr << "Unable to open file: " << filename << std::endl;
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
        std::cerr << "OpenGL: Shader compilation failed: " << infoLog << std::endl;
        return 0;
    }

    return handle;
}

GLuint createShaderProgram(GLuint vertexShaderHandle, GLuint fragmentShaderHandle) {
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShaderHandle);
    glAttachShader(shaderProgram, fragmentShaderHandle);
    glLinkProgram(shaderProgram);

    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "OpenGL: Shader program link failed: " << infoLog << std::endl;
        return 0;
    }

    return shaderProgram;
}

bool setupOpengl() {
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);

    glViewport(0, 0, w, h);

    glGenBuffers(1, &vboHandle);
    glGenBuffers(1, &eboHandle);
    glGenVertexArrays(1, &vaoHandle);

    glBindVertexArray(vaoHandle);
    glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboHandle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindVertexArray(0);

    GLuint vertexShaderHandle = loadShaderFile(GL_VERTEX_SHADER, "default.vsh");
    if (!vertexShaderHandle) {
        std::cerr << "Failed to create vertex shader." << std::endl;
        return false;
    }

    GLuint fragmentShaderHandle = loadShaderFile(GL_FRAGMENT_SHADER, "default.fsh");
    if (!fragmentShaderHandle) {
        std::cerr << "Failed to create fragment shader." << std::endl;
        return false;
    }

    shaderProgram = createShaderProgram(vertexShaderHandle, fragmentShaderHandle);
    if (!shaderProgram) {
        std::cerr << "Failed to create shader program." << std::endl;
    }

    vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");

    return true;
}

void render() {
    glClearColor(0.787, 0.944, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    GLfloat timeValue = glfwGetTime();
    GLfloat greenValue = (std::sin(timeValue) / 2) + 0.5;
    glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
    glUseProgram(shaderProgram);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBindVertexArray(vaoHandle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboHandle);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

int main() {

    if (!glfwInit()) {
        std::cerr << "GLFW: Init error." << std::endl;
    }

    int major, minor, revision;
    glfwGetVersion(&major, &minor, &revision);
    std::cout << "GLFW: Version: " << major << "." << minor << "." << revision << std::endl;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    int width = 800, height = 600;
    window = glfwCreateWindow(width, height, "Hello, world!", nullptr, nullptr);
    if (!window) {
        std::cerr << "GLFW: CreateWindow error!" << std::endl;

        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    const GLubyte* gl_version = glGetString(GL_VERSION);
    std::cout << "OpenGL: Version: " << gl_version << std::endl;

    GLint nrAttibutes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttibutes);
    std::cout << "OpenGL: Maximum number of attributes: " << nrAttibutes << std::endl;

    int frameBufferWidth, frameBufferHeight;
    glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
    std::cout << "GLFW: Framebuffer size: (" << frameBufferWidth << ", " << frameBufferHeight << ")" << std::endl;

    if (!setupOpengl()) {
        std::cerr << "OpenGL: Setup error!" << std::endl;

        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    while (true) {
        if (glfwWindowShouldClose(window) == GLFW_TRUE) {
            std::cout << "GLFW: Closing window." << std::endl;
            break;
        }

        render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    if (window) {
        glfwDestroyWindow(window);
        window = nullptr;
    }

    glfwTerminate();

    return 0;
}


