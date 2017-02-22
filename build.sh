#!/bin/sh

mkdir -p build
g++ main.cpp -I/usr/local/include -L/usr/lib -L/usr/local/lib -lglfw3 -framework OpenGL -o build/glfw3-test
