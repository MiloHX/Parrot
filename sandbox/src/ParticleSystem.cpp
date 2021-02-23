#include "ParticleSystem.h"
#include "parrot/tool/Random.h"

#include "glm/gtx/constants.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/compatibility.hpp"

ParticleSystem::ParticleSystem(uint32_t max_particles) : m_pool_index(max_particles - 1) {
    m_particle_pool.resize(max_particles);
    Random::init(); // Temp
}

void ParticleSystem::onUpdate(parrot::TimeStep time_step) {
    for (auto& particle : m_particle_pool) {
        if (!particle.active) {
            continue;
        }
        
        if (particle.life_remaning <= 0.0f) {
            particle.active = false;
            continue;
        }

        particle.life_remaning -= time_step;
        particle.position += glm::vec3{ particle.velocity.x * (float)time_step, particle.velocity.y * (float)time_step, 0.0f };
        particle.rotation += 0.01f * time_step;
    }
}

void ParticleSystem::onRender(parrot::OrthographicCamera& camera) {

    parrot::Renderer2D::beginScene(camera);

    for (auto& particle : m_particle_pool) {
        if (!particle.active) {
            continue;
        }

        // Fade away particles
        float     life  = particle.life_remaning / particle.life_time;
        glm::vec4 color = glm::lerp(particle.color_end, particle.color_begin, life);
        float     size  = glm::lerp(particle.size_end , particle.size_begin , life);
        
        parrot::Renderer2D::drawQuad(
            particle.position,
            glm::vec2{ size, size },
            particle.rotation,
            nullptr,
            color
        );
    }

    parrot::Renderer2D::endScene();
}

void ParticleSystem::emit(const ParticleProps& particle_props) {
    Particle& particle = m_particle_pool[m_pool_index];
    particle.active   = true;
    particle.position = particle_props.position;
    particle.rotation = Random::randomFloat() * 360.0f;

    particle.velocity    = particle_props.velocity;
    particle.velocity.x += particle_props.volocity_variation.x * (Random::randomFloat() - 0.5f);
    particle.velocity.y += particle_props.volocity_variation.y * (Random::randomFloat() - 0.5f);

    particle.color_begin = particle_props.color_begin;
    particle.color_end   = particle_props.color_end;

    particle.life_time     = particle_props.life_time;
    particle.life_remaning = particle_props.life_time;
    
    particle.size_begin = particle.size_begin + particle_props.size_variation * (Random::randomFloat() - 0.5f);
    particle.size_end   = particle.size_end;

    m_pool_index = (--m_pool_index) % m_particle_pool.size();
}