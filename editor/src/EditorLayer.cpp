#include "EditorLayer.h"
#include "parrot/tool/Math.h"

#include <parrot/io/SceneSerializer.h>
#include <parrot/tool/PlatformTool.h>
#include <imgui.h>
#include <ImGuizmo.h>

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

        m_editor_camera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);


        #if 0
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

        #endif
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
            m_editor_camera.setViewportSize(m_viewport_size.x, m_viewport_size.y);
            m_active_scene->onViewportResize((uint32_t)m_viewport_size.x, (uint32_t)m_viewport_size.y);
        }

        {
            PROFILE_SCOPE("CameraController")
            if (m_viewport_focused) {
                m_camera_controller.onUpdate(time_step);
            }
            m_editor_camera.onUpdate(time_step);
        }

        Renderer2D::resetStatics();

        {
            PROFILE_SCOPE("Renderer")
            m_frame_buffer->bind();
            RenderCommand::setClearColor(glm::vec4{ 0.3f, 0.3f, 0.3f, 1.0f });
            RenderCommand::clear();

            m_active_scene->onEditorUpdate(time_step, m_editor_camera);

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

                if (ImGui::MenuItem("New", "Ctrl+N")) {
                    newScene();
                }

                if (ImGui::MenuItem("Open...", "Ctrl+O")) {
                    openScene();
                }

                if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) {
                    saveSceneAs();
                }

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
        Application::get().getImGuiLayer()->blockEvents(!m_viewport_focused && !m_viewport_hovered);

        ImVec2 viewport_panel_size = ImGui::GetContentRegionAvail();
        if (m_viewport_size.x != viewport_panel_size.x || m_viewport_size.y != viewport_panel_size.y) {
            m_viewport_size = glm::vec2(viewport_panel_size.x, viewport_panel_size.y);
        }
        uint32_t texture_id = m_frame_buffer->getColorAttachmentRendererID();
        ImGui::Image((void*)(static_cast<size_t>(texture_id)), ImVec2(m_viewport_size.x, m_viewport_size.y), ImVec2(0, 1), ImVec2(1, 0));

        // Gizmos
        Entity selected_entity = m_hierarchy_panel.getSelectedEntity();
        //auto   camera_entity   = m_active_scene->getActiveCamera();
        //if (selected_entity.valid() && camera_entity.valid() && m_gizmo_type != -1) {
        if (selected_entity.valid() && m_gizmo_type != -1) {
            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();
            float window_width  = (float)ImGui::GetWindowWidth ();
            float window_height = (float)ImGui::GetWindowHeight();
            ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, window_width, window_height);

            // Runtime Camera
            // const auto& camera        = camera_entity.get<CameraComponent>().camera;
            // const glm::mat4& camera_proj = camera.getProjection();
            // glm::mat4        camera_view = glm::inverse(camera_entity.get<TransformComponent>().getTransform());

            // Editor camera
            const glm::mat4& camera_proj = m_editor_camera.getProjection();
            glm::mat4        camera_view = m_editor_camera.getViewMatrix();

            // Entity Transform
            auto&     entity_transform_comp = selected_entity.get<TransformComponent>();
            glm::mat4 entity_transform      = entity_transform_comp.getTransform();

            // Snapping
            bool  snap       = Input::isKeyPressed(KeyCode::Key_Left_Shift) || Input::isKeyPressed(KeyCode::Key_Right_Shift);

            float snap_value = 0.5f;
            if (m_gizmo_type == ImGuizmo::OPERATION::ROTATE) {
                snap_value = 45.0f;
            }
            float snap_values[3] = { snap_value, snap_value, snap_value };

            ImGuizmo::Manipulate(
                glm::value_ptr(camera_view),
                glm::value_ptr(camera_proj),
                (ImGuizmo::OPERATION)m_gizmo_type,
                ImGuizmo::LOCAL,
                glm::value_ptr(entity_transform),
                nullptr,
                snap ? snap_values : nullptr
            );

            if (ImGuizmo::IsUsing()) {
                glm::vec3 translation;
                glm::vec3 rotation;
                glm::vec3 scale;
                decomposeGlmTransform(entity_transform, translation, rotation, scale);
                entity_transform_comp.translation = translation;
                entity_transform_comp.scale       = scale      ;

                glm::vec3 delta_rotation = rotation - entity_transform_comp.rotation;
                entity_transform_comp.rotation += delta_rotation;
            }

        }


        ImGui::End();
        ImGui::PopStyleVar();


        // Dockable Windows End
        ImGui::End();
    }

    void EditorLayer::onEvent(Event& event) {
        m_camera_controller.onEvent(event);
        m_editor_camera.onEvent(event);

        EventDispatcher dispatcher(event);
        dispatcher.dispatch<KeyPressedEvent>(PR_BIND_EVENT_FUNC(EditorLayer::onKeyPressed));
    }

    bool EditorLayer::onKeyPressed(KeyPressedEvent& event) {
        if (event.getRepeatCount() > 0) {
            return false;
        }

        bool control = Input::isKeyPressed(KeyCode::Key_Left_Control) || Input::isKeyPressed(KeyCode::Key_Right_Control);
        bool shift   = Input::isKeyPressed(KeyCode::Key_Left_Shift  ) || Input::isKeyPressed(KeyCode::Key_Right_Shift  );
        switch (event.getKeyCode()) {
            case (int)KeyCode::Key_S: {
                if (control && shift) {
                    saveSceneAs();
                }
                break; 
            }
            case (int)KeyCode::Key_N: {
                if (control) {
                    newScene();
                }
                break; 
            }   
            case (int)KeyCode::Key_O: {
                if (control) {
                    openScene();
                }
                break; 
            }
            case (int)KeyCode::Key_Q: {
                m_gizmo_type = -1;
                break;
            }
            case (int)KeyCode::Key_W: {
                m_gizmo_type = ImGuizmo::OPERATION::TRANSLATE;
                break;
            }
            case (int)KeyCode::Key_E: {
                m_gizmo_type = ImGuizmo::OPERATION::ROTATE;
                break;
            }
            case (int)KeyCode::Key_R: {
                m_gizmo_type = ImGuizmo::OPERATION::SCALE;
                break;
            }
            break;
        }
    }

    void EditorLayer::newScene() {
        m_active_scene = createRef<Scene>();
        m_active_scene->onViewportResize((uint32_t)m_viewport_size.x, (uint32_t)m_viewport_size.y);
        m_hierarchy_panel.setScene(m_active_scene);
    }

    void EditorLayer::openScene() {
        std::string file_path = FileDialogs::openFile("Parrot Scene (*.parrot)\0*.parrot\0");
        if (!file_path.empty()) {
            m_active_scene = createRef<Scene>();
            m_active_scene->onViewportResize((uint32_t)m_viewport_size.x, (uint32_t)m_viewport_size.y);
            m_hierarchy_panel.setScene(m_active_scene);

            SceneSerializer serializer(m_active_scene);
            serializer.deserialize(file_path);
        }
    }

    void EditorLayer::saveSceneAs() {
        std::string file_path = FileDialogs::saveFile("Parrot Scene (*.parrot)\0*.parrot\0");
        if (!file_path.empty()) {
            SceneSerializer serializer(m_active_scene);
            serializer.serialize(file_path);
        }
    }

}