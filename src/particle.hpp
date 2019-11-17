#ifndef PARTICLE_H
#define PARTICLE_H

#include "GL/glut.h"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

typedef glm::vec3 vec3;
typedef glm::vec4 vec4;

#include "enums.hpp"

class Emitter;

class Particle {
    public:
        Particle(
            Emitter* emitter,
            vec3 ipos,
            vec3 ivel,
            vec4 icol,
            vec4 ecol,
            float lifetime
        );

        ~Particle();

        void Update(float dt);
        void DrawPoint();
        void DrawQuad(vec3 right, vec3 up, float size);

        Particle* prev;
        Particle* next;

        static int count;

        vec3 pos;
        vec3 vel;
        vec4 color;
        vec4 d_color;

        float lifetime;
        float age;
        Emitter* emitter;
};

#endif