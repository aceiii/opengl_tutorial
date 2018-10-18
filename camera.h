#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum Directions {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
};

class Camera {
public:
    void init(float screenWidth, float screenHeight, float fov, glm::vec3 pos);
    void move(Directions dir, float amount);
    void moveTarget(float xoffset, float yoffset);
    float getFieldOfView() const;
    void setFieldOfView(float fov);
    glm::mat4 getProjectionMatrix() const;
    glm::mat4 getViewMatrix() const;
    glm::vec3 getPosition() const;

private:
    void rebuildViewMatrix();
    void rebuildProjectionMatrix();

private:
    glm::mat4 _proj;
    glm::mat4 _view;

    glm::vec3 _pos;
    glm::vec3 _up;
    glm::vec3 _front;

    float _yaw = 0;
    float _pitch = 0;
    float _fov = 0;
    float _nearPlane = 0;
    float _farPlane = 0;
    float _width = 0;
    float _height = 0;
};


#endif// __CAMERA_H__
