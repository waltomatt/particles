#include "particle.hpp"
#include "emitter.hpp"
#include "game.hpp"
#include <iostream>

#include "SOIL/SOIL.h"
#include "glm/glm.hpp"

using namespace std;

int Particle::count = 0;


Particle::Particle(Emitter* emitter, vec3 startPos, vec3 startVel, float size, vec4 startColor, vec4 endColor, GLfloat lifetime) {
    this->pos = startPos;
    this->vel = startVel;
    this->col = startColor;
    this->dcol = (endColor - startColor) * (1.0f/lifetime);
    this->size = size;

    this->type = ParticleType::BILLBOARD;
    
 
    this->lifetime = lifetime;
    this->age = 0.0f;

    this->prev = nullptr;
    this->next = nullptr;
    this->emitter = emitter;

    if (emitter->head == NULL) {
        // empty list
        emitter->head = this;
        emitter->tail = this;
    } else {
        emitter->tail->next = this;
        this->prev = emitter->tail;
        emitter->tail = this;
    }

    Particle::count++;
}

Particle::~Particle() {
    // update our pointers for le linked list

    if (this->emitter->head == this)
        this->emitter->head = this->next;

    if (this->emitter->tail == this)
        this->emitter->tail = this->prev;

    if (this->prev != nullptr)
        this->prev->next = this->next;

    if (this->next != nullptr)
        this->next->prev = this->prev;

    Particle::count--;
}

void Particle::UpdateAll(float dt) {
    // go through our linked list and update all the particles
    Particle* ptr = Particle::head;
    while (ptr != nullptr) {
        ptr->Update(dt);
        ptr = ptr->next;
    }
}

void Particle::RemoveAll() {
    Particle* ptr = Particle::head;
    while (ptr != nullptr) {
        Particle* toDelete = ptr;            
        ptr = ptr->next;
        delete toDelete;
    }
}

void Particle::DrawAll() {
    // go through our linked list and draw all the particles
    Particle* ptr = Particle::head;

    
    
    while (ptr != nullptr) {
        ptr->Draw();
        ptr = ptr->next;
    }

    
    
}

void Particle::Update(float dt) {
    // Update position based on velocity
    this->pos = this->pos + (this->vel * dt);
    // Update velocity based on static gravity at the mo
    this->vel = this->vel + (vec3(0.0f, 0.0f, Game::gravity)  * dt);

    // also wind position
    this->vel = this->vel + (Game::wind_direction * Game::wind_strength * dt);

    // update colour based on the differential
    this->col = this->col + (this->dcol * dt);

    // update the age
    this->age += dt;

    // check if we're too old for this anymore
    if (this->age >= this->lifetime)
        delete this;
}

void Particle::Draw() {
    switch(this->type) {
        case ParticleType::POINT:
            glBegin(GL_POINTS);
            glColor4f(this->col.r, this->col.g, this->col.b, this->col.a);
            glVertex3f(this->pos.x, this->pos.y, this->pos.z);
            glEnd();
            break;

        case ParticleType::BILLBOARD:
            float modelView[16];
            int i, j;

            printf("%d\n", tex);

            glGetFloatv(GL_MODELVIEW_MATRIX, modelView);
            
            vec3 right = vec3(modelView[0], modelView[4], modelView[8]);
            vec3 up = vec3(modelView[1], modelView[5], modelView[9]);

            vec3 a = this->pos - right * (size/2);
            vec3 b = this->pos + right * (size/2);
            vec3 c = this->pos + right * (size/2) + up * size;
            vec3 d = this->pos - right * (size/2) + up * size;

            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, tex);


            glBegin(GL_QUADS);
                //glColor4f(this->col.r, this->col.g, this->col.b, this->col.a);
                
                glColor3f(1.0, 0.0, 0.0);
                glTexCoord2f(0, 1);
                glVertex3f(a.x, a.y, a.z);

                glTexCoord2f(1, 1);
                glColor3f(0.0, 1.0, 0.0);
                glVertex3f(b.x, b.y, b.z);

                glTexCoord2f(1, 0);
                glColor3f(0.0, 0.0, 1.0);
                glVertex3f(c.x, c.y, c.z);

                glTexCoord2f(0, 0);
                glColor3f(1.0, 1.0, 1.0);
                glVertex3f(d.x, d.y, d.z);


            glEnd();
            break;

            
    }
    
}
