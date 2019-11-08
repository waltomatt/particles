#ifndef GAME_H
#define GAME_H

#include "GL/glut.h"
#include <glm/vec3.hpp>
#include <chrono>

#define FOV 60
#define DRAW_DISTANCE 200

typedef glm::vec3 vec3;

class Game {
    public:
        static float gravity;
        static vec3 wind_direction;
        static float wind_strength;

        Game(int argc, char** argv);
        //~Game();

        static void Update();
        static void Display();
        static void Reshape(int w, int h);
        static void Keyboard(unsigned char key, int x, int y);

        static std::chrono::high_resolution_clock::time_point last_draw;

};

#endif