#include <iostream>
#include <GLFW/glfw3.h>

void render() {
    glClearColor(0.787, 0.944, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

int main() {

    if (!glfwInit()) {
        std::cerr << "GLFW: Init error." << std::endl;
    }

    int major, minor, revision;
    glfwGetVersion(&major, &minor, &revision);
    std::cout << "GLFW: Version: " << major << "." << minor << "." << revision << std::endl;

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    int width = 800, height = 600;
    GLFWwindow *window = glfwCreateWindow(width, height, "Hello, world!", nullptr, nullptr);
    if (!window) {
        std::cerr << "GLFW: CreateWindow error!" << std::endl;

        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    const GLubyte* gl_version = glGetString(GL_VERSION);
    std::cout << "OpenGL: Version: " << gl_version << std::endl;

    int frameBufferWidth, frameBufferHeight;
    glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
    std::cout << "GLFW: Framebuffer size: (" << frameBufferWidth << ", " << frameBufferHeight << ")" << std::endl;

    glViewport(0, 0, 200, 200);

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


