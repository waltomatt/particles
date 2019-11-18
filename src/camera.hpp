#ifndef CAMERA_H
#define CAMERA_H

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

typedef glm::vec3 vec3;
class GLFWwindow;

class Camera {
    public:
        Camera(GLFWwindow* window);
        
        vec3 pos;
        float yaw;
        float pitch;
        GLFWwindow* window;
        bool context;
        bool disabled;
        glm::vec2 mousepos;

        void Transform();
        void MouseMove(GLFWwindow* window, double x, double y);
        void Update(double dt);
};

#endif