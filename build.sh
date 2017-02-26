#!/bin/sh

mkdir -p build

SOURCE_FILES="main.cpp \
    shader.cpp"

g++ $SOURCE_FILES -I/usr/local/include -L/usr/lib -L/usr/local/lib -lglfw3 -framework OpenGL -o build/glfw3-test
