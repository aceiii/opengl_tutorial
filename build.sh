#!/bin/sh

mkdir -p build

SOURCE_FILES="main.cpp \
    shader.cpp \
    imgui.cpp \
    imgui_demo.cpp \
    imgui_draw.cpp \
    imgui_impl_glfw_gl3.cpp"

g++ -std=c++14 $SOURCE_FILES -I/usr/local/include -L/usr/lib -L/usr/local/lib -lglfw3 -lsoil -framework CoreFoundation -framework OpenGL -o build/glfw3-test
