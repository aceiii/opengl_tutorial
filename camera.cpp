#include "camera.h"

void Camera::init(float screenWidth, float screenHeight, float fov, glm::vec3 pos) {
    _width = screenWidth;
    _height = screenHeight;
    _fov = fov;
    _nearPlane = 0.01f;
    _farPlane = 1000.0f;
    _yaw = -90.0f;
    _pitch = 0.0f;
    _pos = pos;

    _front = glm::vec3(0.0f, 0.0f, -1.0f);
    _up = glm::vec3(0.0f, 1.0f, 0.0f);

    rebuildViewMatrix();
    rebuildProjectionMatrix();
}

void Camera::move(Directions dir, float amount) {
    if (dir == FORWARD) {
        _pos += _front * amount;
    }
    if (dir == BACKWARD) {
        _pos -= _front * amount;
    }
    if (dir == LEFT) {
        _pos -= glm::normalize(glm::cross(_front, _up)) * amount;
    }
    if (dir == RIGHT) {
        _pos += glm::normalize(glm::cross(_front, _up)) * amount;
    }

    rebuildViewMatrix();
}

void Camera::moveTarget(float xoffset, float yoffset) {
    _yaw += xoffset;
    _pitch += yoffset;

    if (_pitch > 89.0f) {
        _pitch = 89.0f;
    } else if (_pitch < -89.0f) {
        _pitch = -89.0f;
    }

    glm::vec3 front;
    front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    front.y = sin(glm::radians(_pitch));
    front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));

    _front = glm::normalize(front);

    rebuildViewMatrix();
}

float Camera::getFieldOfView() const {
    return _fov;
}

void Camera::setFieldOfView(float fov) {
    _fov = fov;

    rebuildProjectionMatrix();
}

glm::mat4 Camera::getProjectionMatrix() const {
    return _proj;
}

glm::mat4 Camera::getViewMatrix() const {
    return _view;
}

void Camera::rebuildViewMatrix() {
    _view = glm::lookAt(_pos, _pos + _front, _up);
}

void Camera::rebuildProjectionMatrix() {
    _proj = glm::perspective(_fov, _width / _height, _nearPlane, _farPlane);
}
