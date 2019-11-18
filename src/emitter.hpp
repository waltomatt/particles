#ifndef EMITTER_H
#define EMITTER_H

#include "GLFW/glfw3.h"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <vector>

#include "enums.hpp"

typedef glm::vec3 vec3;
typedef glm::vec4 vec4;


class Particle;

class Emitter {
    public:
        static std::vector<Emitter*> emitters;

        Emitter(    vec3 pos,

                    vec3 mean_velocity,
                    vec3 velocity_variance,

                    float size,
                    ParticleType type,

                    vec4 mean_color,
                    vec4 end_color,
                    vec4 color_variance,

                    float mean_lifetime,
                    float lifetime_variance,

                    float emission_rate
                );

        ~Emitter();

        int id;
        vec3 pos;
        
        vec3 vel_mean;
        vec3 vel_var;
        bool vel_normal; // normalize the generated vector so emissions are spherical, and then scaled by the variance

        float size;
        ParticleType type;
        GLint texture;

        float life_mean;
        float life_var;

        vec4 color_mean;
        vec4 color_end;
        vec4 color_var;

        float emit_rate;
        int emit_amt;

        void Update(double dt);
        void Emit(int amount);
        void OptionMenu();
        void RenderParticles();
        void RemoveParticles();
        
        static int inc; // incrementing value that will be the ID of our emitter
        static void UpdateAll(double dt);
        static void RenderAll();
        static void RemoveAll();
        static void RenderMenus();
        
        Particle* head;
        Particle* tail;

        private:

            double emit_time;
            double last_emit;

            
};

#endif