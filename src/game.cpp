
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "GL/glew.h"

#include "game.hpp"
#include "particle.hpp"
#include "emitter.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <stdio.h>
#include <algorithm>


double Game::last_update = 0;
bool Game::vsync = 0;
float Game::speed = 1;

GLFWwindow* Game::window = nullptr;

float Game::gravity = -9.81f;
vec3 Game::wind_direction  = vec3(10, 0, 0);
float Game::wind_strength = 0;

void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "glfw error: %s\n", description);
}

Game::Game(int argc, char **argv) {
    
    glfwSetErrorCallback(glfw_error_callback);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    if (argc < 2) // if we have an option, make it fullscreen
        Game::window = glfwCreateWindow(2560, 1600, "Matt's COMP37111 particle simulator", NULL, NULL);
    else
        Game::window = glfwCreateWindow(2560, 1600, "Matt's COMP37111 particle simulator", glfwGetPrimaryMonitor(), NULL); 

    if (!Game::window) {
        fprintf(stderr, "Failed to create glfw window!\n");
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(Game::window);
    glfwSwapInterval(Game::vsync);
    glfwSetWindowSizeCallback(Game::window, Game::Reshape);
    Game::Reshape(Game::window, 1920, 1080);

    Game::InitImgui();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Emitter* emitter = new Emitter(
        vec3(0,0,0),
        vec3(0, 0, -100), vec3(100, 100, -50),
        4, 1,
        vec4(0, 0.3, 1, 1), vec4(1, 1, 1, 0), vec4(0.1, 0.1, 0.1, 0),
        0.5, 0.1,
        1000
    );

    emitter->vel_normal = false;

    while (!glfwWindowShouldClose(Game::window)) {
        // main loop
        
        Game::Display();

        glfwSwapBuffers(Game::window);

        glfwPollEvents();
        Game::Update();
    }

    glfwDestroyWindow(Game::window);
    glfwTerminate();
}

double Game::last_draw = 0;
double Game::frame_time = 0;

void Game::Display() {
    double time_now = glfwGetTime();
    Game::frame_time = time_now - Game::last_draw;
    Game::last_draw = time_now;

    glClear(GL_COLOR_BUFFER_BIT); // clear the screen
    glLoadIdentity(); // load identity matrix


    gluLookAt(  0.0, 230.0, 0.0, 
                0.0, 0.0, 0.0, 
                0.0, 0.0, 1.0   );

    Particle::DrawAll();
    Game::RenderGui();
}

void Game::Reshape(GLFWwindow* window, int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(FOV, (GLfloat)w / (GLfloat)h, 1.0, DRAW_DISTANCE);
    glMatrixMode(GL_MODELVIEW);
}

void Game::Keyboard(unsigned char key, int x, int y) {
    if (key == 32) {
        
    }


    if (key == 27) exit(0);
}

void Game::Update() {
    double time_now = glfwGetTime();


    double dt = time_now - Game::last_update;
    dt = dt * Game::speed;
    Game::last_update = time_now;

    Emitter::UpdateAll(dt);
    Particle::UpdateAll(dt);
}


void Game::InitImgui() {
    glewInit();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(Game::window, true);

    const char* glsl_version = "#version 130";

    ImGui_ImplOpenGL3_Init(glsl_version);
}

vec3 Game::RandVec3() {
    float rx = 1.0f - 2*Game::RandFloat();
    float ry = 1.0f - 2*Game::RandFloat();
    float rz = 1.0f - 2*Game::RandFloat();

    return glm::normalize(vec3(rx, ry, rz));
}

float Game::RandFloat() {
    return ((float)rand() / RAND_MAX);
}

void Game::RenderGui() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Particle Simulator");

    ImGui::SliderFloat("Sim Speed", &Game::speed, 0.000f, 5.0f);
    ImGui::SliderFloat("Gravity", &Game::gravity, -50.f, 50.f);

    ImGui::InputFloat3("Wind dir", glm::value_ptr(Game::wind_direction));
    ImGui::SliderFloat("Wind speed", &Game::wind_strength, 0, 100);

    if (ImGui::Button("Clear particles"))
        Particle::RemoveAll();

    ImGui::End();

    Game::RenderFPS();
    Emitter::RenderMenus();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Game::RenderFPS() {
    ImGui::Begin("Performance");
    ImGui::Text("Particles: %d", Particle::count);
    ImGui::Text("FPS: %.3f (Avg: %.3f)", ((float)1.0f / Game::frame_time), ImGui::GetIO().Framerate);
    if (ImGui::Checkbox("vsync", &Game::vsync))
        glfwSwapInterval(Game::vsync);

    ImGui::End();
}
