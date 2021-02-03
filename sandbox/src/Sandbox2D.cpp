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

    parrot::Renderer2D::resetStatics();

    {
        PROFILE_SCOPE("Renderer")
        parrot::RenderCommand::setClearColor(glm::vec4{ 0.3f, 0.3f, 0.3f, 1.0f });
        parrot::RenderCommand::clear();

        static float animated_rotation = 0.0f;
        animated_rotation += 1; // TODO: For some reason I cannot multiply this with time_step. 

        parrot::Renderer2D::beginScene(m_camera_controller.getCamera());
        parrot::Renderer2D::drawQuad(glm::vec3{ -1.0f,  0.0f,  0.0f }, glm::vec2{ 0.8f,  0.8f  }, animated_rotation, nullptr, glm::vec4{ 1.0f, 1.0f, 0.0f, 1.0f });
        parrot::Renderer2D::drawQuad(glm::vec3{ 0.5f,  -0.5f,  0.0f }, glm::vec2{ 0.5f,  0.75f }, 45.0f, nullptr, glm::vec4{ 0.0f, 0.0f, 1.0f, 1.0f });
        parrot::Renderer2D::drawQuad(
            glm::vec3{ 0.0f, 0.0f, -0.1f },    // position
            glm::vec2{ 15.0f, 15.0f },         // size
            0,                                 // rotation
            m_checkerboard_texture,            // texture
            glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}, // color
            glm::vec2(10.0f)                   // texture_scale
        );
        parrot::Renderer2D::drawQuad(
            glm::vec3{ 0.0f, 0.0f, 0.0f },     // position
            glm::vec2{ 1.0f, 1.0f },           // size
            45.0f,                             // rotation
            m_checkerboard_texture,            // texture
            glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}, // color
            glm::vec2(2.0f)                    // texture_scale
        );  
        parrot::Renderer2D::endScene();

        parrot::Renderer2D::beginScene(m_camera_controller.getCamera());
        for (float y = -5.0f; y < 5.0f; y += 0.5f) {
            for (float x = -5.0f; x < 5.0f; x += 0.5f) {
                glm::vec4 grad_color{ (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
                parrot::Renderer2D::drawQuad(
                    glm::vec3{ x, y, 0.0f },
                    glm::vec2{ 0.45f, 0.45f },
                    0,
                    nullptr,
                    grad_color,
                    glm::vec2{ 1.0f, 1.0f }
                );
            }
        }
        parrot::Renderer2D::endScene();
    }
}

void Sandbox2D::onImGuiRender() {
    ImGui::Begin("Statistics");

    auto stats = parrot::Renderer2D::getStatics();

    ImGui::Text("Renderer2D:");
    ImGui::Text("Draw Calls: %d", stats.draw_calls);
    ImGui::Text("Quad Count: %d", stats.quad_count);

    ImGui::End();
}

void Sandbox2D::onEvent(parrot::Event& event) {
    m_camera_controller.onEvent(event);
}
