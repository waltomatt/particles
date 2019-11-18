#include "camera.hpp"
#include "constants.hpp"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include <stdio.h>

Camera::Camera(GLFWwindow* window) {
    this->pitch = 0;
    this->yaw = 0;

    this->pos = vec3(100, 100, 100);
    this->window = window;
    this->context = false;
    this->disabled = false;
    double x, y;
    glfwGetCursorPos(this->window, &x, &y);
    this->mousepos = glm::vec2(x, y);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
}

void Camera::Transform() {
    // apply camera transformations
    if (this->disabled) return; 
    glRotatef(this->pitch, 1.0, 0.0, 0.0);
    glRotatef(this->yaw, 0.0, 1.0, 0.0);
    glTranslatef(-this->pos.x, -this->pos.y, -this->pos.z);
}
   

void Camera::Update(double dt) {
    if (this->context || this->disabled) return;
    double x, y;
    glfwGetCursorPos(this->window, &x, &y);
    
    double dx, dy;
    dx = x - this->mousepos.x;
    dy = y - this->mousepos.y;

    this->mousepos.x = x;
    this->mousepos.y = y;

    this->yaw += (dx * MOUSE_SPEED * dt);
    this->pitch += (dy * MOUSE_SPEED * dt);

    if (this->pitch > 90)
        this->pitch = 89.5;

    if (this->pitch <-90)
        this->pitch = -89.5;

    // calulcate sin & cos of pitch and yaw
    float cpitch = glm::cos(glm::radians(this->pitch));
    float spitch = glm::sin(glm::radians(this->pitch));
    float cyaw = glm::cos(glm::radians(this->yaw));
    float syaw = glm::sin(glm::radians(this->yaw));

    // craete forward and right directions for movement
    glm::vec3 forward = glm::vec3(
        syaw * cpitch,
        -spitch,
        -cyaw * cpitch
    );

    glm::vec3 right = glm::vec3(
        cyaw,
        0,
        syaw
    );

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        dt = dt * 3;


    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) 
        this->pos += (forward * MOVE_SPEED * (float)dt);
    

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) 
        this->pos -= (forward * MOVE_SPEED * (float)dt);
    
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        this->pos -= (right * MOVE_SPEED * (float)dt);
    
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        this->pos += (right * MOVE_SPEED * (float)dt);
}
