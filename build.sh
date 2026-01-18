#!/bin/sh

mkdir -p build

SOURCE_FILES="build.cpp \
    imgui/imgui_impl_glfw.cpp \
    imgui/imgui_impl_opengl3.cpp \
    glad/glad.c"

g++ -g -std=c++23 -DIMGUI_IMPL_OPENGL_LOADER_GLAD=1 $SOURCE_FILES \
    -I/usr/local/include -I./ -I./assimp/include -I./assimp/build/include \
    -L/usr/lib -L/usr/local/lib -L./assimp/build/lib \
    -lglfw3 -lassimp -lz \
    -framework Cocoa -framework CoreFoundation -framework OpenGL -framework IOKit \
    -o build/glfw3-test
