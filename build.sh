#!/bin/sh

mkdir -p build

SOURCE_FILES="build.cpp \
    imgui/imgui_impl_glfw.cpp \
    imgui/imgui_impl_opengl3.cpp \
    glad/glad.c"

g++ -g -std=c++14 -DIMGUI_IMPL_OPENGL_LOADER_GLAD=1 $SOURCE_FILES -I/usr/local/include -I./ -L/usr/lib -L/usr/local/lib -lglfw3 -lsoil -framework CoreFoundation -framework OpenGL -o build/glfw3-test
