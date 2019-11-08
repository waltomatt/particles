#include "particle.hpp"
#include <iostream>

using namespace std;

Particle* Particle::head = nullptr;
Particle* Particle::tail = nullptr;
int Particle::count = 0;

Particle::Particle(vec3 startPos, vec3 startVel, vec3 startColor, vec3 endColor, GLfloat lifetime) {
    this->pos = startPos;
    this->vel = startVel;
    this->col = startColor;
    this->dcol = (endColor - startColor) * (1.0f/lifetime);
 
    this->lifetime = lifetime;
    this->age = 0.0f;

    this->prev = nullptr;
    this->next = nullptr;

    if (Particle::head == NULL) {
        // empty list
        Particle::head = this;
        Particle::tail = this;
    } else {
        Particle::tail->next = this;
        this->prev = Particle::tail;
        Particle::tail = this;
    }
}

Particle::~Particle() {
    // update our pointers for le linked list

    if (Particle::head == this)
        Particle::head = this->next;

    if (Particle::tail == this)
        Particle::tail = this->prev;

    if (this->prev != nullptr)
        this->prev->next = this->next;

    if (this->next != nullptr)
        this->next->prev = this->prev;
}

void Particle::UpdateAll(float dt) {
    // go through our linked list and update all the particles
    Particle* ptr = Particle::head;
    while (ptr != nullptr) {
        ptr->Update(dt);
        ptr = ptr->next;
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
    this->vel = this->vel + (vec3(0.0f, 0.0f, -9.81f)  * dt);

    // update colour based on the differential
    this->col = this->col + (this->dcol * dt);

    // update the age
    //this->age += dt;

    // check if we're too old for this anymore
    if (this->age >= this->lifetime)
        delete this;
}

void Particle::Draw() {
    glPushMatrix();

    // draw each particle as a small sphere for now
    glTranslatef(this->pos.x, this->pos.y, this->pos.z);
    glColor3f(this->col.r, this->col.g, this->col.b);
    glutSolidSphere(3, 50, 50);

    glPopMatrix(); 
}
