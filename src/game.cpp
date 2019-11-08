#include "game.hpp"
#include "particle.hpp"

using namespace std::chrono;

high_resolution_clock::time_point Game::last_draw = high_resolution_clock::now();

Game::Game(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(1920, 1080);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Matt's COMP37111 Particle Simulator");

    glutDisplayFunc(Game::Display);
    glutReshapeFunc(Game::Reshape);
    glutKeyboardFunc(Game::Keyboard);
    glutIdleFunc(Game::Update);

    Game::last_draw = high_resolution_clock::now();    

    glutMainLoop();
}

void Game::Display() {
    glClear(GL_COLOR_BUFFER_BIT); // clear the screen
    glLoadIdentity(); // load identity matrix


    gluLookAt(  0.0, 110.0, 0.0, 
                0.0, 0.0, 0.0, 
                0.0, 0.0, 1.0   );

    Particle::DrawAll();

    glFlush(); 
}

void Game::Reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(FOV, (GLfloat)w / (GLfloat)h, 1.0, DRAW_DISTANCE);
    glMatrixMode(GL_MODELVIEW);
}

void Game::Keyboard(unsigned char key, int x, int y) {
    if (key == 27) exit(0);
}

void Game::Update() {
    high_resolution_clock::time_point time_now = high_resolution_clock::now();
    duration<float> dur = duration_cast<duration<float>>(time_now - Game::last_draw);
    float dt = dur.count();
    Game::last_draw = time_now;

    Particle::UpdateAll(dt);

    glutPostRedisplay();
}