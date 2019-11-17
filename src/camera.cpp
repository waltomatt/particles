#include "camera.hpp"
#include "constants.hpp"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include <stdio.h>

Camera::Camera(GLFWwindow* window) {
    this->pitch = 0;
    this->yaw = 0;

    this->pos = vec3(0, 0, 0);
    this->window = window;

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
    glfwSetCursorPosCallback(window, this->MouseMove);
}

void Camera::Transform() {
    // apply camera transformations
    glRotatef(this->pitch, 1.0, 0.0, 0.0);
    glRotatef(this->yaw, 0.0, 1.0, 0.0);
    glTranslatef(-this->pos.x, -this->pos.y, -this->pos.z);
}

void Camera::MouseMove(GLFWwindow* window, double x, double y) {
    printf("%f, %f\n", x, y);
    this->yaw += (x * MOUSE_SPEED);
    this->pitch += (y * MOUSE_SPEED);

    if (this->pitch > 90)
        this->pitch = 89.5;

    if (this->pitch <-90)
        this->pitch = -89.5;
}

void Camera::Update(double dt) {

    float dx = glm::sin(glm::radians(this->yaw)) * glm::cos(glm::radians(this->pitch)) * MOVE_SPEED * dt;
    float dy = -glm::sin(glm::radians(this->pitch)) * MOVE_SPEED * dt;
    float dz = -glm::cos(glm::radians(this->yaw)) * glm::cos(glm::radians(this->pitch)) * MOVE_SPEED * dt;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        this->pos.x += dx;
        this->pos.y += dy;
        this->pos.z += dz;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        this->pos.x -= dx;
        this->pos.y -= dy;
        this->pos.z -= dz;
    }

}
