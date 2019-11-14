#ifndef GAME_H
#define GAME_H

#include "GLFW/glfw3.h"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#define FOV 60
#define DRAW_DISTANCE 500

typedef glm::vec3 vec3;
typedef glm::vec4 vec4;

class Game {
    public:
        static float gravity;
        static vec3 wind_direction;
        static float wind_strength;

        static float speed;

        Game(int argc, char** argv);
        //~Game();

        static void Update();
        static void Display();
        static void Reshape(GLFWwindow* window, int w, int h);
        static void Keyboard(unsigned char key, int x, int y);
        static void InitImgui();
        static void RenderGui();

        static void RenderFPS();

        static vec3 RandVec3();
        static float RandFloat();

        static double last_update;
        static double last_draw;
        static double frame_time;

        static bool vsync;

        static GLFWwindow* window;
        

};

#endif