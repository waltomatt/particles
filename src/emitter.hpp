#ifndef EMITTER_H
#define EMITTER_H

#include <glm/vec3.hpp>
#include <vector>

typedef glm::vec3 vec3;
typedef glm::vec4 vec4;

class Particle;

class Emitter {
    public:
        static std::vector<Emitter*> emitters;

        Emitter(    vec3 pos,

                    vec3 mean_velocity,
                    vec3 velocity_variance,

                    float size_mean,
                    float size_var,

                    vec4 mean_color,
                    vec4 end_color,
                    vec4 color_variance,

                    float mean_lifetime,
                    float lifetime_variance,

                    float emission_rate
                );

        int id;
        vec3 pos;
        
        vec3 vel_mean;
        vec3 vel_var;
        bool vel_normal; // normalize the generated vector so emissions are spherical, and then scaled by the variance

        float size_mean;
        float size_var;

        float life_mean;
        float life_var;

        vec4 color_mean;
        vec4 color_end;
        vec4 color_var;

        float emit_rate;

        void Update(double dt);
        void Emit(int amount);
        void OptionMenu();

        static int inc; // incrementing value that will be the ID of our emitter
        static void UpdateAll(double dt);
        static void RenderAll();
        static void DeleteAll();
        static void RenderMenus();
        
        Particle* head;
        Particle* tail;

        private:

            double emit_time;
            double last_emit;

            
};

#endif