#pragma once
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
    glm::vec4                            m_color = glm::vec4{ 0.2f, 0.3f, 0.8f, 1.0f };
};