#include "Sandbox2D.h"

#include <imgui.h>

static const uint32_t s_map_width = 24;
static const char* s_map_tiles = 
    "WWWWWWWWWWWWWWWWWWWWWWWW"
    "WWWWWWWWWWWWWWWWWWWWWWWW"
    "WWWWDDDDDDDDDDDDWWWWWWWW"
    "WWWDDDDDDDWWDDDDDWWWWWWW"
    "WWWDDDDDWWWWDDDDDWWWWWWW"
    "WWWDDDDWWWXWDDDDDWWWWWWW"
    "WWWDDDDDWWWWDDDDDWWWWWWW"
    "WWWDDDDDDDDDDDDDDWWWWWWW"
    "WWWDDDDDDDDDDDDDDWWWWWWW"
    "WWWDDDDDDDDDDDDDDWWWWWWW"
    "WWWWDDDDDDDDDDDDWWWWWWWW"
    "WWWWWDDDDDDDDDDWWWWWWWWW"
    "WWWWWWWWWWWWWWWWWWWWWWWW"
    "WWWWWWWWWWWWWWWWWWWWWWWW"
;

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"),  m_camera_controller(1920.0f / 1080.0f, true), m_map_width(0), m_map_height(0) {
}

void Sandbox2D::onAttach() {
    m_map_width  = s_map_width;
    m_map_height = strlen(s_map_tiles) / m_map_width;

    m_checkerboard_texture = parrot::Texture2D   ::create("asset/texture/Checkerboard.png");
    m_sprite_sheet         = parrot::Texture2D   ::create("asset/game/texture/RPGpack_sheet_2X.png");
    s_textre_map['D']      = parrot::SubTexture2D::createFromCoords(m_sprite_sheet, glm::vec2( 6, 11), glm::vec2(128, 128));
    s_textre_map['W']      = parrot::SubTexture2D::createFromCoords(m_sprite_sheet, glm::vec2(11, 11), glm::vec2(128, 128));
    m_sprite_stair         = parrot::SubTexture2D::createFromCoords(m_sprite_sheet, glm::vec2(7, 6), glm::vec2(128, 128));
    m_sprite_barrel        = parrot::SubTexture2D::createFromCoords(m_sprite_sheet, glm::vec2(8, 2), glm::vec2(128, 128));
    m_sprite_tree          = parrot::SubTexture2D::createFromCoords(m_sprite_sheet, glm::vec2(2, 1), glm::vec2(128, 128), glm::vec2(1, 2));

    m_particle.color_begin        = glm::vec4{ 1.0f, 1.0f, 0.0f, 1.0f };
    m_particle.color_begin        = glm::vec4{ 0.0f, 0.0f, 1.0f, 1.0f };
    m_particle.size_begin         = 0.5f;
    m_particle.size_end           = 0.0f;
    m_particle.size_variation     = 0.3f;
    m_particle.life_time          = 1.0f;
    m_particle.velocity           = glm::vec2{ 0.0f, 0.0f };
    m_particle.volocity_variation = glm::vec2{ 3.0f, 1.0f };
    m_particle.position           = glm::vec3{ 0.0f, 0.0f, 0.0f };

    m_camera_controller.setZoomLevel(3.0f);

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

        #if 0
        parrot::Renderer2D::beginScene(m_camera_controller.getCamera());
        parrot::Renderer2D::drawQuad(glm::vec3{ -1.0f,  0.0f,  0.1f }, glm::vec2{ 0.8f,  0.8f  }, animated_rotation, nullptr, glm::vec4{ 1.0f, 1.0f, 0.0f, 1.0f });
        parrot::Renderer2D::drawQuad(glm::vec3{ 0.5f,  -0.5f,  0.1f }, glm::vec2{ 0.5f,  0.75f }, 45.0f, nullptr, glm::vec4{ 0.0f, 0.0f, 1.0f, 1.0f });
        parrot::Renderer2D::drawQuad(
            glm::vec3{ 0.0f, 0.0f,  -0.2f },    // position
            glm::vec2{ 15.0f, 15.0f },         // size
            0,                                 // rotation
            m_checkerboard_texture,            // texture
            glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}, // color
            glm::vec2(10.0f)                   // texture_scale
        );
        parrot::Renderer2D::drawQuad(
            glm::vec3{ 0.0f, 0.0f, 0.1f },     // position
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
                    glm::vec3{ x, y, -0.1f },
                    glm::vec2{ 0.45f, 0.45f },
                    0,
                    nullptr,
                    grad_color
                );
            }
        }
        parrot::Renderer2D::endScene();
        #endif

        #if 1
        if (parrot::Input::isMouseButtonPressed(parrot::MouseButton::Button_Left)) {
            auto [x, y] = parrot::Input::getMousePosition();
            auto width  = parrot::Application::get().getWindow().getWidth ();
            auto height = parrot::Application::get().getWindow().getHeight();

            auto bounds   = m_camera_controller.getBounds();
            auto position = glm::vec3{ m_camera_controller.getCamera().getPosition().x,m_camera_controller.getCamera().getPosition().y, 0.5f };
            x = (x / width) * bounds.getWidth() - bounds.getWidth() * 0.5f;
            y = bounds.getHeight() * 0.5f - (y / height) * bounds.getHeight();
            m_particle.position = glm::vec3{ x + position.x, y + position.y, position.z};
            for (int i = 0; i < 5; ++i) {
                m_particle_system.emit(m_particle);
            }
        }

        m_particle_system.onUpdate(time_step);
        m_particle_system.onRender(m_camera_controller.getCamera());
        #endif


        #if 1
        parrot::Renderer2D::beginScene(m_camera_controller.getCamera());

        for (uint32_t y = 0; y < m_map_height; ++y) {
            for (uint32_t x = 0; x < m_map_width; ++x) {
                char tile_type = s_map_tiles[x + y * m_map_width];
                parrot::Ref<parrot::SubTexture2D> texture;
                if (s_textre_map.find(tile_type) != s_textre_map.end()) {
                    texture = s_textre_map[tile_type];
                } else {
                    texture = m_sprite_barrel;
                }
                parrot::Renderer2D::drawSubTexture(
                    glm::vec3{ x - m_map_width/2.0f, m_map_height/2.0f - y, 0.0f },     // position
                    glm::vec2{ 1.0f, 1.0f },                                            // size
                    0,                                                                  // rotation
                    texture                                                             // texture
                );
            }
        }

        parrot::Renderer2D::endScene();

        #endif

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
