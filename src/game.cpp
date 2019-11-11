#include "game.hpp"
#include "particle.hpp"

#include <stdio.h>


double Game::last_draw = 0;
GLFWwindow* Game::window = nullptr;

void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "glfw error: %s\n", description);
}

Game::Game(int argc, char **argv) {
    
    glfwSetErrorCallback(glfw_error_callback);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    Game::window = glfwCreateWindow(1920, 1080, "Matt's COMP37111 particle simulator", NULL, NULL);

    if (!Game::window) {
        fprintf(stderr, "Failed to create glfw window!\n");
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(Game::window);
    glfwSetWindowSizeCallback(Game::window, Game::Reshape);
    Game::Reshape(Game::window, 1920, 1080);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (int i=0; i<10000; i++) {
        float rx = ((float)rand() / RAND_MAX) - 0.5f;
        float ry = ((float)rand() / RAND_MAX) - 0.5f;
        float rz = ((float)rand() / RAND_MAX) - 0.5f;


        Particle* particle = new Particle(
            vec3(0, 0, 0),
            vec3(15 * rx, 15 * ry, 15 * rz),
            vec4(0, 1, 0, 1),
            vec4(1, 0, 0, 0),
            5.0f,
            0.1f,
            4
        );
    }

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

void Game::Display() {
    glClear(GL_COLOR_BUFFER_BIT); // clear the screen
    glLoadIdentity(); // load identity matrix


    gluLookAt(  0.0, 230.0, 0.0, 
                0.0, 0.0, 0.0, 
                0.0, 0.0, 1.0   );

    Particle::DrawAll();

    glFlush(); 
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
    float dt = Game::last_draw - time_now;
    Game::last_draw = time_now;

    Particle::UpdateAll(dt);
}