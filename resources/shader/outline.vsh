#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float outlineSize;

mat4 scale(vec3 scale_value) {
    return mat4(
        scale_value.x, 0.0, 0.0, 0.0,
        0.0, scale_value.y, 0.0, 0.0,
        0.0, 0.0, scale_value.z, 0.0,
        0.0, 0.0, 0.0, 1.0);
}

void main()
{
    float _scale = 0.1 * outlineSize;
    //gl_Position = projection * view * model * scale(vec3(_scale)) * vec4(position, 1.0f);
    gl_Position = projection * view * model * vec4(position + (normal * _scale), 1.0f);
}

