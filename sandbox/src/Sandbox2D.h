#pragma once

#include "ParticleSystem.h"
#include <Parrot.h>

class Sandbox2D : public parrot::Layer {
public:
    Sandbox2D();
    virtual ~Sandbox2D() = default;

    void onAttach() override;
    void onDetach() override;
    void onUpdate(parrot::TimeStep time_step) override;
    void onImGuiRender() override;
    void onEvent(parrot::Event& event) override;
private:
    parrot::OrthographicCameraController m_camera_controller;
    parrot::Ref<parrot::VertexArray>     m_vertex_array;
    parrot::Ref<parrot::Shader>          m_flat_color_shader;
    parrot::Ref<parrot::Texture2D>       m_checkerboard_texture;
    parrot::Ref<parrot::Texture2D>       m_sprite_sheet;
    parrot::Ref<parrot::SubTexture2D>    m_sprite_stair;
    parrot::Ref<parrot::SubTexture2D>    m_sprite_barrel;
    parrot::Ref<parrot::SubTexture2D>    m_sprite_tree;
    glm::vec4                            m_color = glm::vec4{ 0.2f, 0.3f, 0.8f, 1.0f };

    ParticleSystem m_particle_system;
    ParticleProps  m_particle;
};