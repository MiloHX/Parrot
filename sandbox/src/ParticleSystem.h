#pragma once

#include <Parrot.h>

struct ParticleProps {
    glm::vec3 position;
    glm::vec2 velocity;
    glm::vec2 volocity_variation;
    glm::vec4 color_begin;
    glm::vec4 color_end;
    float size_begin;
    float size_end;
    float size_variation;
    float life_time = 1.0f;
};


class ParticleSystem {
public:
    ParticleSystem(uint32_t max_particles = 1000);
    void onUpdate(parrot::TimeStep time_step);
    void onRender(parrot::OrthographicCamera& camera);
    void emit    (const ParticleProps& particle_props);
private:
    struct Particle {
        glm::vec3 position;
        glm::vec2 velocity;
        glm::vec4 color_begin;
        glm::vec4 color_end;
        float rotation = 0.0f;
        float size_begin;
        float size_end;
        float life_time = 1.0f;
        float life_remaning = 0.0f;
        bool  active = false;
    };
    std::vector<Particle> m_particle_pool;
    uint32_t m_pool_index;
};