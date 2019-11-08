#ifndef PARTICLE_H
#define PARTICLE_H

#include "GL/glut.h"
#include <glm/vec3.hpp>

typedef glm::vec3 vec3;

class Particle {
    public:
        Particle(   vec3 ipos,
                    vec3 ivel,
                    vec3 icol,
                    vec3 ecol,
                    GLfloat lifetime
        );

        ~Particle();

        static void UpdateAll(float dt);
        static void DrawAll();
        
        void Update(float dt);
        void Draw();

        Particle* prev;
        Particle* next;

        static Particle* head;
        static Particle* tail;

        static int count;

    private:
        vec3 pos;
        vec3 vel;
        vec3 col;
        vec3 dcol;
        float lifetime; // lifetime in ms
        float age; // age in ms

};

#endif