#include "EditorLayer.h"

#include <imgui.h>

namespace parrot {

    EditorLayer::EditorLayer() : Layer("Editor"),  m_camera_controller(1920.0f / 1080.0f, true) {
    }

    void EditorLayer::onAttach() {

        m_checkerboard_texture = Texture2D::create("asset/texture/Checkerboard.png");

        FrameBufferProps frame_buffer_props;
        frame_buffer_props.width  = 1280;
        frame_buffer_props.height = 720;
        m_frame_buffer = FrameBuffer::create(frame_buffer_props);
    }

    void EditorLayer::onDetach() {
    }


    void EditorLayer::onUpdate(TimeStep time_step) {
        PROFILE_SCOPE("Sandbox2D::onUpdate")

        {
            PROFILE_SCOPE("CameraController")
            if (m_viewport_focused) {
                m_camera_controller.onUpdate(time_step);
            }
        }

        Renderer2D::resetStatics();

        {
            PROFILE_SCOPE("Renderer")
            m_frame_buffer->bind();
            RenderCommand::setClearColor(glm::vec4{ 0.3f, 0.3f, 0.3f, 1.0f });
            RenderCommand::clear();

            Renderer2D::beginScene(m_camera_controller.getCamera());

            parrot::Renderer2D::drawQuad(
                glm::vec3{ 0.0f, 0.0f, 0.0f },     // position
                glm::vec2{ 1.0f, 1.0f },           // size
                0,                                 // rotation
                m_checkerboard_texture,            // texture
                glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}, // color
                glm::vec2(1.0f)                    // texture_scale
            );

            Renderer2D::endScene();

            m_frame_buffer->unbind();
        }
    }

    void EditorLayer::onImGuiRender() {

        // Docking
        static bool dockspace_open = true;
        static bool opt_fullscreen_persistant = true;
        bool opt_fullscreen = opt_fullscreen_persistant;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

        if (opt_fullscreen) {
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos     (viewport->Pos );
            ImGui::SetNextWindowSize    (viewport->Size);
            ImGui::SetNextWindowViewport(viewport->ID  );
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding  , 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }

        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) {
            window_flags |= ImGuiWindowFlags_NoBackground;
        }

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("Parrot", &dockspace_open, window_flags);
        ImGui::PopStyleVar();

        if (opt_fullscreen) {
            ImGui::PopStyleVar(2);
        }

        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
            ImGuiID dockspace_id = ImGui::GetID("Parrot");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("FILE")) {

                if (ImGui::MenuItem("Test Item 1 (NoSplit)", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0))
                    dockspace_flags ^= ImGuiDockNodeFlags_NoSplit;
                if (ImGui::MenuItem("Test Item 2 (NoResize)", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0))
                    dockspace_flags ^= ImGuiDockNodeFlags_NoResize;

                ImGui::Separator();
                if (ImGui::MenuItem("Exit", NULL, false)) {
                    dockspace_open = false;
                    parrot::Application::get().exit();
                }
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        // Dockable Windows Need to put here:

        // Statistics
        ImGui::Begin("Statistics");
        auto stats = parrot::Renderer2D::getStatics();
        ImGui::Text("Renderer2D:");
        ImGui::Text("Draw Calls: %d", stats.draw_calls);
        ImGui::Text("Quad Count: %d", stats.quad_count);
        ImGui::End();

        // Viewport
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::Begin("Viewport");

        m_viewport_focused = ImGui::IsWindowFocused();
        m_viewport_hovered = ImGui::IsWindowHovered();
        Application::get().getImGuiLayer()->blockEvents(!m_viewport_focused || !m_viewport_hovered);

        ImVec2 viewport_panel_size = ImGui::GetContentRegionAvail();
        if (m_viewport_size.x != viewport_panel_size.x || m_viewport_size.y != viewport_panel_size.y) {
            m_viewport_size = glm::vec2(viewport_panel_size.x, viewport_panel_size.y);
            m_frame_buffer->resize(static_cast<uint32_t>(m_viewport_size.x), static_cast<uint32_t>(m_viewport_size.y));

            m_camera_controller.onResize(m_viewport_size.x, m_viewport_size.y);
        }
        uint32_t texture_id = m_frame_buffer->getColorAttachmentRendererID();
        ImGui::Image((void*)texture_id, ImVec2(m_viewport_size.x, m_viewport_size.y), ImVec2(0, 1), ImVec2(1, 0));
        ImGui::End();
        ImGui::PopStyleVar();

        // Dockable Windows End
        ImGui::End();
    }

    void EditorLayer::onEvent(Event& event) {
        m_camera_controller.onEvent(event);
    }

}