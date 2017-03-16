#include <iostream>
#include <fstream>
#include <string>
#include <complex>

#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

#include <SOIL.h>

#include "shader.h"

namespace {
    GLFWwindow *window = nullptr;

    GLfloat vertices[] = {
		// Positions          // Colors           // Texture Coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Top Right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Bottom Right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // Bottom Left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // Top Left
    };
    GLuint indices[] = {
        0, 1, 3,
		1, 3, 2,
    };

    GLuint vboHandle, vaoHandle, eboHandle;
    GLuint vertexColorLocation, textureLocation0, textureLocation1;
    GLuint texture0, texture1;

    Shader shader;
}

bool loadTexture(const std::string &name, GLuint textureId) {
    int width, height;
    unsigned char *image = SOIL_load_image(name.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
    if (!image) {
        std::cerr << "loadTexture: Unable to load file '" << name << "'" << std::endl;
        return false;
    }

    glBindTexture(GL_TEXTURE_2D, textureId);
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        if (error == GL_INVALID_VALUE) {
            std::cerr << "loadTexture: textureId is invalid" << std::endl;
        }

        SOIL_free_image_data(image);
        glBindTexture(GL_TEXTURE_2D, 0);
        return false;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboHandle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindVertexArray(0);

    if (!shader.init("default.vsh", "default.fsh")) {
        std::cerr << "Failed to initialize shaders." << std::endl;
        return false;
    }

    vertexColorLocation = glGetUniformLocation(shader.program, "ourColor");
    textureLocation0 = glGetUniformLocation(shader.program, "ourTexture1");
    textureLocation1 = glGetUniformLocation(shader.program, "ourTexture2");

    glGenTextures(1, &texture0);
    glGenTextures(1, &texture1);

    if (!loadTexture("container.jpg", texture0)) {
        return false;
    }

    if (!loadTexture("awesomeface.png", texture1)) {
        return false;
    }

    return true;
}

void render() {
    glClearColor(0.787, 0.944, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    GLfloat timeValue = glfwGetTime();
    GLfloat greenValue = (std::sin(timeValue) / 2) + 0.5;
    glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture0);
    glUniform1i(textureLocation1, 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glUniform1i(textureLocation1, 1);

    shader.use();

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


