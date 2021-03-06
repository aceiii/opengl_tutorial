#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <memory>

#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>
#include "glad/glad.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "fmt/core.h"
#include "fmt/ostream.h"

#include "shader.h"
#include "camera.h"
#include "light.h"
#include "model.h"
#include "mesh.h"
#include "texture.h"

namespace {
    GLFWwindow *window = nullptr;

	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
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


    GLfloat lastX = 800.0f / 2.0f;
    GLfloat lastY = 600.0f / 2.0f;

    GLuint vboHandle, vaoHandle, lightVaoHandle;
    GLuint texture0, texture1, texture2;

    Shader shader;
    Shader lampShader;

    bool hide_cursor = true;
    bool keys[1024];

    GLfloat lastFrame = 0.0f;
    GLfloat deltaTime = 0.0f;

    const int WIDTH = 800;
    const int HEIGHT = 600;

    Camera camera;

    glm::vec3 clearColor(0.164f, 0.240f, 0.407f);
    float materialShininess = 32.0f;

    GLfloat lampScale = 0.2f;

    bool forceQuit = false;
    bool enableDiffuse = true;
    bool enableSpecular = true;
    bool enableEmissive = false;
    bool animateLamp = true;

    Lights lights {1};

    Shader modelShader;
    Shader outlineShader;
    std::unique_ptr<Model> model;

    glm::vec3 modelPosition {0.0f, -1.75f, 2.5f};
    float modelScale = 0.15f;

    bool modelOutline = false;
    glm::vec3 outlineColor {1.0f, 0.0f, 0.0f};
    float outlineSize = 1.0f;
}

/*
bool loadTexture(const std::string &name, GLuint textureId) {
    int width, height;
    unsigned char *image = SOIL_load_image(name.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
    if (!image) {
        fmt::print(std::cerr, "loadTexture: Unable to load file '{}'\n", name);
        return false;
    }

    glBindTexture(GL_TEXTURE_2D, textureId);
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        if (error == GL_INVALID_VALUE) {
            fmt::print(std::cerr, "loadTexture: textureId is invalid\n");
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
*/

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    if (!shader.init("resources/shader/lighting.vsh", "resources/shader/lighting.fsh")) {
        fmt::print(std::cerr, "Failed to initialize default shaders.\n");
        return false;
    }

    glGenTextures(1, &texture0);
    glGenTextures(1, &texture1);
    glGenTextures(1, &texture2);

    if (!textureFromFile("resources/texture/container2.png", texture0)) {
        return false;
    }

    if (!textureFromFile("resources/texture/container2_specular.png", texture1)) {
        return false;
    }

    if (!textureFromFile("resources/texture/matrix.jpg", texture2)) {
        return false;
    }

    /// Initialize light cube =================================================
    glGenVertexArrays(1, &lightVaoHandle);

    glBindVertexArray(lightVaoHandle);
    glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    if (!lampShader.init("resources/shader/lamp.vsh", "resources/shader/lamp.fsh")) {
        fmt::print(std::cerr, "Failed to initialize lamp shaders.\n");
        return false;
    }

    /// Initialize mesh =======================================================
    model = std::make_unique<Model>("resources/model/nanosuit/nanosuit.obj");
    //model = std::make_unique<Model>("resources/model/Homer_simpson/Homer_Simpson.blend");
    if (!model) {
        fmt::print(std::cerr, "Failed to initialize nanosuit model.\n");
        return false;
    }

    if (!modelShader.init("resources/shader/model.vsh", "resources/shader/model.fsh")) {
        fmt::print(std::cerr, "Failed to initialize model shaders.\n");
        return false;
    }

    if (!outlineShader.init("resources/shader/outline.vsh", "resources/shader/outline.fsh")) {
        fmt::print(std::cerr, "Failed to initialize outline shaders.\n");
        return false;
    }

    return true;
}

void render() {
    glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    GLfloat timeValue = glfwGetTime();

    glm::mat4 projection = camera.getProjectionMatrix();
    glm::mat4 view = camera.getViewMatrix();

    // Draw container cubes

    shader.use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, enableDiffuse ? texture0 : 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, enableSpecular ? texture1 : 0);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, enableEmissive ? texture2 : 0);

    lights.set(shader);

    shader.setInt("material.diffuse", 0);
    shader.setInt("material.specular", 1);
    shader.setInt("material.emissive", 2);
    shader.setFloat("material.shininess", materialShininess);

    shader.setVec3("viewPos", camera.getPosition());
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);

    glBindVertexArray(vaoHandle);
    for (GLuint i = 0; i < 10; i += 1) {
        glm::mat4 model;
        model = glm::translate(model, cubePositions[i]);

        GLfloat angle = cubeRotations[i] * GLfloat(timeValue);
        model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));

        shader.setMat4("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // draw mesh
    glm::mat4 matModel;
    matModel = glm::translate(matModel, modelPosition);
    matModel = glm::scale(matModel, glm::vec3(modelScale));

    modelShader.use();

    lights.set(modelShader);

    modelShader.setVec3("viewPos", camera.getPosition());
    modelShader.setMat4("projection", projection);
    modelShader.setMat4("view", view);
    modelShader.setMat4("model", matModel);

    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);

    model->draw(shader);

    if (modelOutline) {
        outlineShader.use();
        outlineShader.setVec3("viewPos", camera.getPosition());
        outlineShader.setMat4("projection", projection);
        outlineShader.setMat4("view", view);
        outlineShader.setMat4("model", matModel);
        outlineShader.setVec3("outlineColor", outlineColor);
        outlineShader.setFloat("outlineSize", outlineSize);

        glStencilFunc(GL_NOTEQUAL, 1,  0xFF);
        glStencilMask(0x00);
        glDisable(GL_DEPTH_TEST);

        model->draw(outlineShader);
    }

    glStencilMask(0xFF);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);

    // Draw light sources

    lampShader.use();
    lampShader.setMat4("projection", projection);
    lampShader.setMat4("view", view);

    for (size_t i = 0; i < lights.count(); i += 1) {
        const Light &light = lights.at(i);

        glm::mat4 lampModel;
        lampModel = glm::translate(lampModel, light.position);
        lampModel = glm::scale(lampModel, glm::vec3(lampScale));

        lampShader.setMat4("model", lampModel);
        lampShader.setVec3("lightColor", light.diffuse);

        glBindVertexArray(lightVaoHandle);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glBindVertexArray(0);
}

void renderImGui() {
    if (ImGui::Begin("OpenGL", nullptr, ImGuiWindowFlags_MenuBar)) {
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Quit")) {
                    forceQuit = true;
                }

                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        if (ImGui::TreeNode("Global")) {
            ImGui::ColorEdit3("clear", (float*)&clearColor);
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Camera")) {
            glm::vec3 pos = camera.getPosition();
            if (ImGui::DragFloat3("position", (float*)&pos), 0.1f) {
                camera.setPosition(pos);
            }
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Material")) {
            ImGui::Checkbox("enable diffuse", &enableDiffuse);
            ImGui::Checkbox("enable specular", &enableSpecular);
            ImGui::Checkbox("enable emissive", &enableEmissive);
            ImGui::SliderFloat("shininess", &materialShininess, 4.0f, 128.0f);
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Model")) {
            ImGui::DragFloat3("position", (float*)&modelPosition, 0.1f);
            ImGui::DragFloat("scale", &modelScale, 0.001f, 0.001f, 1.0f);
            ImGui::Checkbox("outline", &modelOutline);
            if (modelOutline) {
                ImGui::PushID("outline");
                ImGui::DragFloat("size", &outlineSize, 0.1f, 0.1f, 10.0f);
                ImGui::ColorEdit3("color", (float*)&outlineColor);
                ImGui::PopID();
            }
            ImGui::TreePop();
        }
        for (size_t i = 0; i < lights.count(); i += 1) {
            Light &light = lights.at(i);

            if (ImGui::TreeNode(fmt::format("Light[{}]", i).c_str())) {
                ImGui::Checkbox("enable", &light.enable);
                ImGui::RadioButton("type point", (int*)&light.type, 0);
                ImGui::RadioButton("type directional", (int*)&light.type, 1);
                ImGui::RadioButton("type spot", (int*)&light.type, 2);
                if (light.type == Light::POINT) {
                    ImGui::Checkbox("animate", &animateLamp);
                    ImGui::DragFloat3("position", (float*)&light.position, 0.1f);
                } else if (light.type == Light::DIRECTIONAL) {
                    ImGui::DragFloat3("direction", (float*)&light.direction, 0.1f);
                } else if (light.type == Light::SPOT) {
                    ImGui::DragFloat3("position", (float*)&light.position, 0.1f);
                    ImGui::DragFloat3("direction", (float*)&light.direction, 0.1f);
                }
                ImGui::ColorEdit3("ambient", (float*)&light.ambient);
                ImGui::ColorEdit3("diffuse", (float*)&light.diffuse);
                ImGui::ColorEdit3("specular", (float*)&light.specular);
                ImGui::DragFloat("strength", &light.strength, 0.01f, 0.0f, 32.0f);
                ImGui::DragFloat("constant", &light.constant, 0.0f, 1.0f, 1.0f);
                ImGui::DragFloat("linear", &light.linear, 0.01f, 0.0f, 1.0f);
                ImGui::DragFloat("quadratic", &light.quadratic, 0.01f, 0.0f, 1.0f);
                if (light.type == Light::SPOT) {
                    ImGui::DragFloat("cutoff", &light.cutoff, 0.001f, 0.44f, 1.0f);
                    ImGui::DragFloat("outer cutoff", &light.outerCutoff, 0.001f, 0.44f, 1.0f);
                    light.outerCutoff = std::min(light.outerCutoff, light.cutoff);
                }
                ImGui::TreePop();
            }
        }
        if (!lights.full()) {
            if (ImGui::Button("Add Lights")) {
                lights.addLight();
            }
        }
    }
    ImGui::End();
}

int main() {

    if (!glfwInit()) {
        std::cerr << "GLFW: Init error." << std::endl;
    }

    int major, minor, revision;
    glfwGetVersion(&major, &minor, &revision);
    fmt::print("GLFW: Version: {}.{}.{}\n", major, minor, revision);

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
        fmt::print(std::cerr, "Failed to initialize OpenGL context\n");
        return -1;
    }

    const GLubyte* gl_version = glGetString(GL_VERSION);
    fmt::print("OpenGL: Version: {}\n", gl_version);

    GLint nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    fmt::print("OpenGL: Maximum number of attributes: {}\n", nrAttributes);

    int frameBufferWidth, frameBufferHeight;
    glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
    fmt::print("GLFW: Framebuffer size: ({}, {})\n", frameBufferWidth, frameBufferHeight);

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

    camera.init(WIDTH, HEIGHT, 45.0F, glm::vec3(-1.75f, 0.0f, 5.0f));
    camera.moveTarget(15.0, -5.0);

    //ImGuiIO &io = ImGui::GetIO();
    //io.MouseDrawCursor = true;

    while (!forceQuit) {
        if (glfwWindowShouldClose(window) == GLFW_TRUE) {
            fmt::print("GLFW: Closing window.\n");
            forceQuit = true;
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


