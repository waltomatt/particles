#include "emitter.hpp"
#include "game.hpp"
#include "particle.hpp"

#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/common.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "imgui/imgui.h"

#include <stdio.h>
#include <string>
#include <algorithm>


std::vector<Emitter*> Emitter::emitters;
int Emitter::inc = 0; // auto incrementing ID for emitter


Emitter::Emitter(   vec3 pos, 
                    vec3 mean_velocity, vec3 velocity_variance,
                    float size,
                    ParticleType type,
                    vec4 mean_color, vec4 end_color, vec4 color_variance,
                    float mean_lifetime, float lifetime_variance,
                    float emission_rate
) {
    this->pos = pos;
    this->vel_mean = mean_velocity;
    this->vel_var = velocity_variance;
    this->vel_normal = true; // default: normalize ejection velocities

    this->size = size;
    this->type = type;

    if (type == ParticleType::BILLBOARD)
        this->texture = 1; // first texture loaded is default

    this->color_mean = mean_color;
    this->color_end = end_color;
    this->color_var = color_variance;

    this->life_mean = mean_lifetime;
    this->life_var = lifetime_variance;

    this->emit_rate = emission_rate;
    this->emit_time = (float)1.0f/emission_rate; // freq -> period
    this->last_emit = glfwGetTime();

    this->id = Emitter::inc++; // set ID and increment

    // head and tail of linked list of particles
    this->head = nullptr;
    this->tail = nullptr;

    // emit amount for emit function
    this->emit_amt = 0;

    Emitter::emitters.push_back(this); // add adr of this emitter to the vector
}

Emitter::~Emitter() {
    Emitter::emitters.erase(std::find(Emitter::emitters.begin(), Emitter::emitters.end(), this));

    // delete all the particles
    this->RemoveParticles();
}

void Emitter::RemoveParticles() {
    Particle* ptr = this->head;
    Particle* to_del = nullptr;

    while (ptr != nullptr) {
        to_del = ptr;
        ptr = ptr->next;
        delete to_del;
    }
}

void Emitter::Update(double dt) {
    // do emissions
    double time_now = glfwGetTime();

    double change = (time_now - this->last_emit);
    change = change * Game::speed;

    int amt = glm::floor(change / this->emit_time);
    if (amt > 0) {
        this->Emit(amt);
        this->last_emit = time_now;
    }

    // update each particle

    Particle* ptr = this->head;

    while (ptr != nullptr) {
        ptr->Update(dt);
        ptr = ptr->next;
    }
}

// static function to update all particles
void Emitter::UpdateAll(double dt) {
    for (auto &emitter: Emitter::emitters) {
        if (emitter != nullptr)
            emitter->Update(dt);
    }
}

void Emitter::Emit(int amount) {
    for (int i=0; i<amount; i++) {
        vec3 vel = this->vel_mean;

        if (this->vel_normal)
            vel = vel + (Game::RandVec3() * this->vel_var);
        else
            vel = vel + ((1.0f-(2*Game::RandFloat())) * this->vel_var);

        Particle* particle = new Particle(
            this,
            this->pos,
            vel, 
            this->color_mean + (1.0f-2*Game::RandFloat()) * this->color_var,
            this->color_end + (1.0f-2*Game::RandFloat()) * this->color_var,
            this->life_mean + (Game::RandFloat() * this->life_var)
        );

  
    }
}

void Emitter::RenderParticles() {
    Particle* ptr = this->head;

    // trying to limit the number of rendercalls for performance
    // doing one glBegin per emitter
    // means we can have billboards and point particles in the same scene, but still maintain decent performance
    if (this->type == ParticleType::POINT) {
        glPointSize(this->size);
        glDisable(GL_TEXTURE_2D);

        glBegin(GL_POINTS);

        while (ptr != nullptr) {
            ptr->DrawPoint();
            ptr = ptr->next;
        }

        glEnd();
                
    } else if (this->type == ParticleType::BILLBOARD) {
        float modelView[16];

        glGetFloatv(GL_MODELVIEW_MATRIX, modelView); // get the modelview matrix

        vec3 right = vec3(modelView[0], modelView[4], modelView[8]); // calculate the camera angle right vector
        vec3 up = vec3(modelView[1], modelView[5], modelView[9]); // and up vector

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, this->texture); // set the texture
        
        glBegin(GL_QUADS);
            while (ptr != nullptr) {
                ptr->DrawQuad(right, up, size);
                ptr = ptr->next;
            }
        glEnd();
        glDisable(GL_TEXTURE_2D);
    }

    
}

void Emitter::RenderAll() {
    for (auto &emitter: Emitter::emitters) {
        if (emitter != nullptr)
            emitter->RenderParticles();
    }
}

void Emitter::RemoveAll() {
    for (auto &emitter: Emitter::emitters) {
        if (emitter != nullptr)
            delete emitter;
    }
}


void Emitter::OptionMenu() {
    char name[14];
    sprintf(name, "Emitter #%d", this->id);
    ImGui::Begin(name);
    if (ImGui::Button("Clear Particles"))
        this->RemoveParticles();
        
    ImGui::InputFloat3("pos", glm::value_ptr(this->pos), "%.1f");
    ImGui::SliderFloat("size", &this->size, 1, 10);

    if (ImGui::RadioButton("Point", (int*)&this->type, (int)ParticleType::POINT)) {
        this->type = ParticleType::POINT;
    }

    ImGui::SameLine();

    if (ImGui::RadioButton("Billboard", (int*)&this->type, (int)ParticleType::BILLBOARD))
        this->type = ParticleType::BILLBOARD;

    if (this->type == ParticleType::BILLBOARD) {
        if (ImGui::BeginCombo("Texture", Game::textures[1])) {
            for (int i=1; i<=Game::texture_count; i++) {
                bool is_selected = (this->texture == i);
                if (ImGui::Selectable(Game::textures[i], is_selected))
                    this->texture = i;

                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }

    ImGui::Text("Emit Velocities");
    ImGui::InputFloat3("vel_mean", glm::value_ptr(this->vel_mean), "%.1f");
    ImGui::InputFloat3("vel_var", glm::value_ptr(this->vel_var), "%.1f");
    ImGui::Checkbox("vel_normal", &this->vel_normal);

    ImGui::Text("Rate");
    if (ImGui::InputFloat("emit_rate", &this->emit_rate))
        this->emit_time = (1.0f/this->emit_rate);

    ImGui::Text("Lifetime");
    ImGui::InputFloat("life_mean", &this->life_mean);
    ImGui::InputFloat("life_var", &this->life_var);


    ImGui::Text("Colors");
    ImGui::ColorPicker4("color_mean", glm::value_ptr(this->color_mean));
    ImGui::InputFloat4("color_var", glm::value_ptr(this->color_var), "%.1f");
    ImGui::ColorPicker4("color_end", glm::value_ptr(this->color_end));
    
    
    ImGui::InputInt("Amount", &this->emit_amt);
    ImGui::SameLine();
    if (ImGui::Button("Emit!"))
        this->Emit(this->emit_amt);

    ImGui::End();
}

void Emitter::RenderMenus() {
    for (auto &emitter: Emitter::emitters) {
        if (emitter != nullptr)
            emitter->OptionMenu();
    }
}