#include "Sandbox2D.h"
#include <imgui.h>

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"),  m_camera_controller(1920.0f / 1080.0f, true) {
}

void Sandbox2D::onAttach() {
    m_checkerboard_texture = parrot::Texture2D::create("asset/texture/Checkerboard.png");
}

void Sandbox2D::onDetach() {
}

void Sandbox2D::onUpdate(parrot::TimeStep time_step) {
    PROFILE_SCOPE("Sandbox2D::onUpdate")

    {
        PROFILE_SCOPE("CameraController")
        m_camera_controller.onUpdate(time_step);
    }

    {
        PROFILE_SCOPE("Renderer")
        parrot::RenderCommand::setClearColor(glm::vec4{ 0.3f, 0.3f, 0.3f, 1.0f });
        parrot::RenderCommand::clear();

        parrot::Renderer2D::beginScene(m_camera_controller.getCamera());
        parrot::Renderer2D::drawQuad(glm::vec3{ -1.0f,  0.0f,  0.0f }, glm::vec2{ 0.8f,  0.8f  },      0, nullptr, glm::vec4{ 1.0f, 1.0f, 0.0f, 1.0f });
        parrot::Renderer2D::drawQuad(glm::vec3{ 0.5f,  -0.5f,  0.0f }, glm::vec2{ 0.5f,  0.75f }, -30.0f, nullptr, glm::vec4{ 0.0f, 0.0f, 1.0f, 1.0f });
        parrot::Renderer2D::drawQuad(
            glm::vec3{ -5.0f, -5.0f, -0.1f },  // position
            glm::vec2{ 10.0f, 10.0f },         // size
            60.0f,                             // rotation
            m_checkerboard_texture,            // texture
            glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}, // color
            glm::vec2(10.0f)                   // texture_scale
        );
        parrot::Renderer2D::drawQuad(
            glm::vec3{ -0.5f, -0.5f, 0.0f },   // position
            glm::vec2{ 1.0f, 1.0f },           // size
            60.0f,                             // rotation
            m_checkerboard_texture,            // texture
            glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}, // color
            glm::vec2(2.0f)                    // texture_scale
        );  
        parrot::Renderer2D::endScene();
    }
}

void Sandbox2D::onImGuiRender() {
}

void Sandbox2D::onEvent(parrot::Event& event) {
    m_camera_controller.onEvent(event);
}
