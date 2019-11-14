#include "game.hpp"
#include "emitter.hpp"
#include "particle.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "imgui/imgui.h"

#include <stdio.h>
#include <string>

typedef glm::vec4 vec4;

std::vector<Emitter*> Emitter::emitters;
int Emitter::inc = 0;

Emitter::Emitter(   vec3 pos, 
                    vec3 mean_velocity, vec3 velocity_variance,
                    float size_mean, float size_var,
                    vec4 mean_color, vec4 end_color, vec4 color_variance, 
                    float mean_life, float life_variance, float emission_rate ) {


    this->pos = pos;

    this->vel_mean = mean_velocity;
    this->vel_var = velocity_variance;
    this->vel_normal = true;

    this->size_mean = size_mean;
    this->size_var = size_var;

    this->color_mean = mean_color;
    this->color_end = end_color;
    this->color_var = color_variance;

    this->life_mean = mean_life;
    this->life_var = life_variance;

    this->emit_rate = emission_rate;
    this->emit_time = (float)1.0f/emission_rate; // convert from frequency to period
    this->last_emit = glfwGetTime();

    this->id = Emitter::inc++;

    Emitter::emitters.push_back(this);

}

void Emitter::Update(double dt) {
    double time_now = glfwGetTime();

    double change = (time_now - this->last_emit);
    change = change * Game::speed;

    int amt = glm::floor(change / this->emit_time);
    if (amt > 0) {
        this->Emit(amt);
        this->last_emit = time_now;
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
            vel, // Pinitial = Pmean + (Rand * Pvariance)
            this->size_mean + (1.0f-2*Game::RandFloat() * this->size_var),
            this->color_mean + (1.0f-2*Game::RandFloat()) * this->color_var,
            this->color_end + (1.0f-2*Game::RandFloat()) * this->color_var,
            this->life_mean + (Game::RandFloat() * this->life_var)
        );

  
    }
}

void Emitter::UpdateAll(double dt) {
    for (auto &emitter: Emitter::emitters) {
        if (emitter != nullptr)
            emitter->Update(dt);
    }
}

void Emitter::OptionMenu() {
    char name[14];
    sprintf(name, "Emitter #%d", this->id);
    ImGui::Begin(name);
    ImGui::InputFloat3("pos", glm::value_ptr(this->pos), "%.1f");
    ImGui::SliderFloat("size_mean", &this->size_mean, 1, 10);
    ImGui::SliderFloat("size_var", &this->size_var, 1, 10);
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

    ImGui::ColorPicker4("color_mean", glm::value_ptr(this->color_mean));

    ImGui::End();
}

void Emitter::RenderMenus() {
    for (auto &emitter: Emitter::emitters) {
        if (emitter != nullptr)
            emitter->OptionMenu();
    }
}