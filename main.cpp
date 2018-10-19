#include <iostream>
#include <fstream>
#include <string>
#include <complex>

#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>
#include "glad/glad.h"

#include <SOIL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "shader.h"
#include "camera.h"

namespace {
    GLFWwindow *window = nullptr;

	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	glm::vec3 cubePositions[] = {
		glm::vec3( 0.0f,  0.0f,  0.0f),
		glm::vec3( 2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3( 2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3( 1.3f, -2.0f, -2.5f),
		glm::vec3( 1.5f,  2.0f, -2.5f),
		glm::vec3( 1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f),
	};

    GLfloat cubeRotations[] = {
        glm::radians(20.0f),
        glm::radians(32.0f),
        glm::radians(-32.0f),
        glm::radians(50.0f),
        glm::radians(-50.0f),
        glm::radians(128.0f),
        glm::radians(90.0f),
        glm::radians(-90.0f),
        glm::radians(-128.0f),
        glm::radians(13.3f),
    };

    GLfloat lampScale = 0.2f;
    glm::vec3 lampPosition(3.0f, 2.3f, -1.4f);

    GLfloat lastX = 800.0f / 2.0f;
    GLfloat lastY = 600.0f / 2.0f;

    GLuint vboHandle, vaoHandle, eboHandle, lightVaoHandle;
    GLuint textureLocation0, textureLocation1;
    GLuint texture0, texture1;

    Shader shader;
    Shader lampShader;

    bool hide_cursor = true;
    bool keys[1024];

    GLfloat lastFrame = 0.0f;
    GLfloat deltaTime = 0.0f;

    const int WIDTH = 800;
    const int HEIGHT = 600;

    Camera camera;

    glm::vec3 clearColor(0.787f, 0.944f, 1.0f);
    glm::vec3 materialAmbient(1.0f, 0.5f, 0.31f);
    glm::vec3 materialDiffuse(1.0f, 0.5f, 0.31f);
    glm::vec3 materialSpecular(1.0f, 0.5f, 0.31f);
    float materialShininess = 32.0f;

    glm::vec3 lightDiffuse(1.0f, 0.35f, 0.65f);
    glm::vec3 lightAmbient(0.4f, 0.14f, 0.26f);
    glm::vec3 lightSpecular(1.0f, 1.0f, 1.0f);
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

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {

    std::cout << "keypress: " << key << ", " <<  scancode << '\n';

    if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE) {
        hide_cursor = !hide_cursor;
        if (hide_cursor) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }

    if (action == GLFW_PRESS) {
        keys[key] = true;
    } else if (action == GLFW_RELEASE) {
        keys[key] = false;
    }
}

bool firstMouse = true;
void mouse_callback(GLFWwindow *window, double xpos, double ypos) {

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    GLfloat sensitivity = 0.05;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    if (hide_cursor) {
        camera.moveTarget(xoffset, yoffset);
    }

}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    float fov = camera.getFieldOfView();

    if (fov >= 1.0f && fov <= 45.0f) {
        fov -= yoffset;
    }
    if (fov <= 1.0f) {
        fov = 1.0f;
    }
    if (fov >= 45.0f) {
        fov = 45.0f;
    }

    camera.setFieldOfView(fov);
}

void do_movement() {
    const GLfloat cameraSpeed = 5.0f * deltaTime;

    if (keys[GLFW_KEY_W]) {
        camera.move(FORWARD, cameraSpeed);
    }
    if (keys[GLFW_KEY_S]) {
        camera.move(BACKWARD, cameraSpeed);
    }
    if (keys[GLFW_KEY_A]) {
        camera.move(LEFT, cameraSpeed);
    }
    if (keys[GLFW_KEY_D]) {
        camera.move(RIGHT, cameraSpeed);
    }

}

bool setupOpengl() {
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);

    glViewport(0, 0, w, h);

    /// Initialize container ==================================================

    glGenBuffers(1, &vboHandle);
    //glGenBuffers(1, &eboHandle);
    glGenVertexArrays(1, &vaoHandle);

    glBindVertexArray(vaoHandle);
    glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    //glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    if (!shader.init("resources/shader/lighting.vsh", "resources/shader/lighting.fsh")) {
        std::cerr << "Failed to initialize default shaders." << std::endl;
        return false;
    }

    textureLocation0 = glGetUniformLocation(shader.program, "ourTexture1");
    textureLocation1 = glGetUniformLocation(shader.program, "ourTexture2");

    glGenTextures(1, &texture0);
    glGenTextures(1, &texture1);

    if (!loadTexture("resources/texture/container.jpg", texture0)) {
        return false;
    }

    if (!loadTexture("resources/texture/awesomeface.png", texture1)) {
        return false;
    }

    /// Initialize light cube =================================================
    glGenVertexArrays(1, &lightVaoHandle);

    glBindVertexArray(lightVaoHandle);
    glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    if (!lampShader.init("resources/shader/lamp.vsh", "resources/shader/lamp.fsh")) {
        std::cerr << "Failed to initialize lamp shaders." << std::endl;
        return false;
    }

    return true;
}

void render() {
    //glClearColor(0.787, 0.944, 1.0, 1.0);
    glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);

    GLfloat timeValue = glfwGetTime();

    // Draw container cubes

    shader.use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    //shader.setInt("ourTexture1", 0);
    //shader.setInt("ourTexture2", 1);

    glm::mat4 projection = camera.getProjectionMatrix();
    glm::mat4 view = camera.getViewMatrix();

    glm::mat4 lampModel;
    lampModel = glm::rotate(lampModel, GLfloat(timeValue) * glm::radians(32.0f), glm::vec3(-0.3, 1, 0));
    lampModel = glm::translate(lampModel, lampPosition);
    lampModel = glm::scale(lampModel, glm::vec3(lampScale));

    shader.setVec3("material.ambient", materialAmbient);
    shader.setVec3("material.diffuse", materialDiffuse);
    shader.setVec3("material.specular", materialSpecular);
    shader.setFloat("material.shininess", materialShininess);

    shader.setVec3("light.ambient", lightAmbient);
    shader.setVec3("light.diffuse", lightDiffuse);
    shader.setVec3("light.specular", lightSpecular);
	shader.setVec3("light.position", glm::vec3(lampModel * glm::vec4(lampPosition, 1.0)));

    shader.setVec3("viewPos", camera.getPosition());
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBindVertexArray(vaoHandle);

    for (GLuint i = 0; i < 10; i += 1) {
        glm::mat4 model;
        model = glm::translate(model, cubePositions[i]);

        GLfloat angle = cubeRotations[i] * GLfloat(timeValue);
        model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));

        shader.setMat4("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // Draw lamp

    lampShader.use();
    lampShader.setMat4("projection", projection);
    lampShader.setMat4("view", view);
    lampShader.setMat4("model", lampModel);

    glBindVertexArray(lightVaoHandle);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);
}

void renderImGui() {
    //ImGui::ShowTestWindow();

    ImGui::Begin("OpenGL");
    ImGui::ColorEdit3("clear", (float*)&clearColor);
    ImGui::Text("Material");
    ImGui::PushID("material");
    {
        ImGui::ColorEdit3("ambient", (float*)&materialAmbient);
        ImGui::ColorEdit3("diffuse", (float*)&materialDiffuse);
        ImGui::ColorEdit3("specular", (float*)&materialSpecular);
        ImGui::SliderFloat("shininess", &materialShininess, 4.0f, 128.0f);
    }
    ImGui::PopID();
    ImGui::Text("Light");
    ImGui::PushID("light");
    {
        ImGui::ColorEdit3("ambient", (float*)&lightAmbient);
        ImGui::ColorEdit3("diffuse", (float*)&lightDiffuse);
        ImGui::ColorEdit3("specular", (float*)&lightSpecular);
    }
    ImGui::PopID();
    ImGui::End();
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

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cerr << "Failed to initialize OpenGL context" << std::endl;
        return -1;
    }

    const GLubyte* gl_version = glGetString(GL_VERSION);
    std::cout << "OpenGL: Version: " << gl_version << std::endl;

    GLint nrAttibutes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttibutes);
    std::cout << "OpenGL: Maximum number of attributes: " << nrAttibutes << std::endl;

    int frameBufferWidth, frameBufferHeight;
    glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
    std::cout << "GLFW: Framebuffer size: (" << frameBufferWidth << ", " << frameBufferHeight << ")" << std::endl;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, false);
    ImGui_ImplOpenGL3_Init("#version 330");

    if (!setupOpengl()) {
        std::cerr << "OpenGL: Setup error!" << std::endl;

        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    camera.init(WIDTH, HEIGHT, 45.0F, glm::vec3(0.0f, 0.0f, 3.0f));


    //ImGuiIO &io = ImGui::GetIO();
    //io.MouseDrawCursor = true;

    while (true) {
        if (glfwWindowShouldClose(window) == GLFW_TRUE) {
            std::cout << "GLFW: Closing window." << std::endl;
            break;
        }

        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        do_movement();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        render();

        if (!hide_cursor) {
            renderImGui();
        }

        ImGui::Render();
        glfwMakeContextCurrent(window);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);
    }

    if (window) {
        glfwDestroyWindow(window);
        window = nullptr;
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();

    return 0;
}


