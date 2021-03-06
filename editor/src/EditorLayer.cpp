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
        m_active_scene = createRef<Scene>();

        m_square_entity = m_active_scene->createEntity("Colored Square");
        m_square_entity.add<SpriteRendererComponent>(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));

        auto square_entity_2 = m_active_scene->createEntity("Colored Square 2");
        square_entity_2.add<SpriteRendererComponent>(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));

        m_camera_primary = m_active_scene->createEntity("Primary Camera Entity");
        m_camera_primary.add<CameraComponent>();

        m_camera_secondary = m_active_scene->createEntity("Secondary Camera Entity");
        m_camera_secondary.add<CameraComponent>();

        m_active_scene->setActiveCamera(m_camera_primary);
        m_use_primary_camera = true;

        class CameraController : public ScriptableEntity {
        public:
            void onCreate() {
            }

            void onDestroy() {
            }

            void onUpdate(TimeStep time_step) {

                auto& translation = get<TransformComponent>().translation;
                float speed = 5.0f;
                if (Input::isKeyPressed(KeyCode::Key_A)) {
                    translation.x -= speed * time_step;
                }
                if (Input::isKeyPressed(KeyCode::Key_D)) {
                    translation.x += speed * time_step;
                }
                if (Input::isKeyPressed(KeyCode::Key_W)) {
                    translation.y += speed * time_step;
                }
                if (Input::isKeyPressed(KeyCode::Key_S)) {
                    translation.y -= speed * time_step;
                }
            }
        };

        m_camera_secondary.add<ScriptComponent>().bind<CameraController>();

        m_hierarchy_panel.setScene(m_active_scene);
    }

    void EditorLayer::onDetach() {
    }


    void EditorLayer::onUpdate(TimeStep time_step) {
        PROFILE_SCOPE("EditorLayer::onUpdate")

        FrameBufferProps props = m_frame_buffer->getFrameBufferProps();

        if (m_viewport_size.x > 0.0f && m_viewport_size.y > 0.0f && (props.width != m_viewport_size.x || props.height != m_viewport_size.y)) {
            m_frame_buffer->resize((uint32_t)m_viewport_size.x, (uint32_t)m_viewport_size.y);
            m_camera_controller.onResize(m_viewport_size.x, m_viewport_size.y);
            m_active_scene->onViewportResize((uint32_t)m_viewport_size.x, (uint32_t)m_viewport_size.y);
        }

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

            m_active_scene->onUpdate(time_step);

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
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
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

        ImGuiIO&   io    = ImGui::GetIO();
        ImGuiStyle& style = ImGui::GetStyle();
        float min_window_size_x = style.WindowMinSize.x;
        style.WindowMinSize.x = 400.0f;
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
            ImGuiID dockspace_id = ImGui::GetID("Parrot");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }
        style.WindowMinSize.x = min_window_size_x;

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

        // Heirarchy

        m_hierarchy_panel.onImGuiRender();

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
        }
        uint32_t texture_id = m_frame_buffer->getColorAttachmentRendererID();
        ImGui::Image((void*)(static_cast<size_t>(texture_id)), ImVec2(m_viewport_size.x, m_viewport_size.y), ImVec2(0, 1), ImVec2(1, 0));
        ImGui::End();
        ImGui::PopStyleVar();


        // Dockable Windows End
        ImGui::End();
    }

    void EditorLayer::onEvent(Event& event) {
        m_camera_controller.onEvent(event);
    }

}