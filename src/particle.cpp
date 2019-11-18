#include "particle.hpp"
#include "emitter.hpp"
#include "game.hpp"

int Particle::count = 0;

Particle::Particle(Emitter* emitter, vec3 startPos, vec3 startVel, vec4 startColor, vec4 endColor, float lifetime) {
    this->pos = startPos;
    this->vel = startVel;
    this->color = startColor;
    this->d_color = (endColor - startColor) * (1.0f/lifetime);

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


void Particle::Update(float dt) {
    // Update position based on velocity
    this->pos = this->pos + (this->vel * dt);
    // Update velocity based on static gravity at the mo
    this->vel = this->vel + (vec3(0.0f, Game::gravity, 0.0f)  * dt);

    // also wind position
    this->vel = this->vel + (Game::wind_direction * Game::wind_strength * dt);

    // update colour based on the differential
    this->color = this->color + (this->d_color * dt);

    // update the age
    this->age += dt;

    // check if we're too old for this anymore
    if (this->age >= this->lifetime)
        delete this;
}

void Particle::DrawPoint() {
    glColor4f(this->color.r, this->color.g, this->color.b, this->color.a);
    glVertex3f(this->pos.x, this->pos.y, this->pos.z);
}

// We're not sorting these so we're gonna have alpha issues, sorting will be quite intensive imo

void Particle::DrawQuad(vec3 right, vec3 up, float size) {
    // calculate vectors for 4 points of the quad
    vec3 bottom_left = this->pos - right * (size/2);
    vec3 bottom_right = this->pos + right * (size/2);
    vec3 top_left = this->pos - right * (size/2) + up * size;
    vec3 top_right = this->pos + right * (size/2) + up * size;

    glColor4f(this->color.r, this->color.g, this->color.b, this->color.a);
    
    glTexCoord2f(0, 1);
    glVertex3f(bottom_left.x, bottom_left.y, bottom_left.z);

    glTexCoord2f(1, 1);
    glVertex3f(bottom_right.x, bottom_right.y, bottom_right.z);

    

    glTexCoord2f(1, 0);
    glVertex3f(top_right.x, top_right.y, top_right.z);

    glTexCoord2f(0, 0);
    glVertex3f(top_left.x, top_left.y, top_left.z);

}

