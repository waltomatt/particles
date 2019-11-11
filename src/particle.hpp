#ifndef PARTICLE_H
#define PARTICLE_H

#include "GL/glut.h"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

typedef glm::vec3 vec3;
typedef glm::vec4 vec4;

class Particle {
    public:
        Particle(   vec3 ipos,
                    vec3 ivel,
                    vec4 icol,
                    vec4 ecol,
                    float isize,
                    float esize,
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
        vec4 col;
        vec4 dcol;

        float size;
        float dsize;

        float lifetime; // lifetime in ms
        float age; // age in ms

};

#endif