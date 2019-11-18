#ifndef GAME_H
#define GAME_H

#include "GLFW/glfw3.h"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <vector>
#include "enums.hpp"
#include "constants.hpp"

typedef glm::vec3 vec3;
typedef glm::vec4 vec4;

class Camera;

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
        static void Keyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void InitImgui();
        static void RenderGui();
        static void RenderAxis();

        static void InitScene();
        static void UpdateScene(double dt);
        static void RenderScene();
        static void RenderSceneGui();

        static GLint LoadTexture(char* name);

        static void RenderFPS();

        static vec3 RandVec3();
        static float RandFloat();

        static double last_update;
        static double last_draw;
        static double frame_time;

        static bool vsync;
        static bool context;
        static bool axis;

        static DemoType demo;

        static GLFWwindow* window;
        static Camera* camera;

        static int texture_count;
        static char* textures[100]; // index is textureID, val is name/path
        

};

#endif